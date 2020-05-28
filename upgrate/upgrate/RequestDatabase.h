#pragma once

#include <unordered_map>
#include <string_view>

struct DatabaseRequest;
using DatabaseRequestHolder = unique_ptr<DatabaseRequest>;

struct DatabaseRequest {
	enum class Type {
		BUS,
		STOP,
		ROUTE
	};

	DatabaseRequest(Type type) : type(type) {}
	static DatabaseRequestHolder Create(Type type);
	virtual void ParseForm(const Json::Node& input) = 0;
	virtual ~DatabaseRequest() = default;

	const Type type;
};

const unordered_map<string_view, DatabaseRequest::Type> STR_TO_DATABASE_REQUEST_TYPE = {
	{"Bus", DatabaseRequest::Type::BUS},
	{"Stop", DatabaseRequest::Type::STOP},
	{"Route", DatabaseRequest::Type::ROUTE}
};


template <typename ResultType>
struct ReadDatabaseRequest : DatabaseRequest {
	using DatabaseRequest::DatabaseRequest;
	virtual ResultType Process(DirectoryTransport& directory) const = 0;
};

//struct ModifyRequest : DatabaseRequest {
//    using DatabaseRequest::DatabaseRequest;
//    virtual void Process(DirectoryTransport& directory) const = 0;
//};

struct ComputeBusDatabaseRequest : ReadDatabaseRequest<string> {
	ComputeBusDatabaseRequest() : ReadDatabaseRequest<string>(Type::BUS) {}

	void ParseForm(const Json::Node& input) override {
		const map<string, Json::Node>& m = input.AsMap();
		name = m.at("name").AsString();
		id = m.at("id").AsInt();
	}

	string Process(DirectoryTransport& directory) const override {
		BusInfo info = directory.CalcBusInfo(name);
		stringstream stream;

		if (info.stopCount == 0) {
			stream << "{";
			stream << "\"request_id\": " << id << ", ";
			stream << "\"error_message\": \"not found\"";
			stream << "}";
		}
		else {
			stream << "{";
			stream << "\"stop_count\": " << info.stopCount << ", ";
			stream << "\"route_length\": " << info.lengthRoute << ", ";
			stream << "\"unique_stop_count\": " << info.uniqueStopCount << ", ";
			stream << "\"curvature\": " << info.curvature << ", ";
			stream << "\"request_id\": " << id;
			stream << "}";
		}
		return stream.str();
	}

	int id;
	string name;
};

struct ComputeStopDatabaseRequest : ReadDatabaseRequest<string> {
	ComputeStopDatabaseRequest() : ReadDatabaseRequest<string>(Type::STOP) {}

	void ParseForm(const Json::Node& input) override {
		const map<string, Json::Node>& m = input.AsMap();
		name = m.at("name").AsString();
		id = m.at("id").AsInt();
	}

	string Process(DirectoryTransport& directory) const override {
		auto[isFound, buses] = directory.CalcStopInfo(name);
		stringstream stream;
		stream << "{";
		stream << "\"request_id\": " << id << ", ";

		if (!isFound) {
			stream << "\"error_message\": \"not found\"";
		}
		else if (buses.empty()) {
			stream << "\"buses\": []";
		}
		else {
			stream << "\"buses\": [";
			bool first = true;
			for (auto& bus : buses) {
				if (first) {
					stream << "\"" << bus << "\"";
					first = false;
				}
				else {
					stream << ", ";
					stream << "\"" << bus << "\"";
				}
			}
			stream << "]";
		}
		stream << "}";

		return stream.str();
	}

	int id;
	string name;
};

struct ComputeRouteDatabaseRequest : ReadDatabaseRequest<string> {
	ComputeRouteDatabaseRequest() : ReadDatabaseRequest<string>(Type::ROUTE) {}

	void ParseForm(const Json::Node& input) override {
		const map<string, Json::Node>& m = input.AsMap();
		from = m.at("from").AsString();
		to = m.at("to").AsString();
		id = m.at("id").AsInt();
	}

	string Process(DirectoryTransport& directory) const override {
		DirectoryTransport::RouteResult routeRes = directory.CalcRoute(from, to);

		stringstream stream;
		stream << "{";

		if (routeRes.items.empty() && !routeRes.isCorrect) {
			stream << "\"request_id\": " << id << ", ";
			stream << "\"error_message\": \"not found\"";
		}
		else {
			stream << "\"total_time\": " << routeRes.total_time << ", ";
			stream << "\"items\": [";

			bool first = true;
			for (auto& item : routeRes.items) {
				if (first) {
					first = false;
				}
				else {
					stream << ", ";
				}

				stream << "{";
				if (item.type == DirectoryTransport::RouteItemType::BUS) {
					stream << "\"span_count\": " << item.count << ", ";
					stream << "\"bus\": \"" << item.name << "\", ";
					stream << "\"type\": " << "\"Bus\"" << ", ";
					stream << "\"time\": " << item.time;
				}
				else if (item.type == DirectoryTransport::RouteItemType::WAIT) {
					stream << "\"time\": " << item.time << ", ";
					stream << "\"type\": " << "\"Wait\"" << ", ";
					stream << "\"stop_name\": \"" << item.name << "\"";
				}
				stream << "}";
			}
			stream << "], ";
			stream << "\"request_id\": " << id;
		}
		stream << "}";

		return stream.str();
	}

	int id;
	string from, to;
};

DatabaseRequestHolder DatabaseRequest::Create(DatabaseRequest::Type type) {
	switch (type) {
	case DatabaseRequest::Type::BUS:
		return make_unique<ComputeBusDatabaseRequest>();
	case DatabaseRequest::Type::STOP:
		return make_unique<ComputeStopDatabaseRequest>();
	case DatabaseRequest::Type::ROUTE:
		return make_unique<ComputeRouteDatabaseRequest>();
	default:
		return nullptr;
	}
}

optional<DatabaseRequest::Type> ConvertDatabaseRequestTypeFromString(string_view type_str) {
	if (const auto it = STR_TO_DATABASE_REQUEST_TYPE.find(type_str);
		it != STR_TO_DATABASE_REQUEST_TYPE.end()) {
		return it->second;
	}
	else {
		return nullopt;
	}
}

DatabaseRequestHolder ParseDatabaseRequest(const Json::Node& node_) {
	const map<string, Json::Node>& node = node_.AsMap();
	const auto request_type = ConvertDatabaseRequestTypeFromString(node.at("type").AsString());
	if (!request_type) {
		return nullptr;
	}
	DatabaseRequestHolder request = DatabaseRequest::Create(*request_type);
	if (request) {
		request->ParseForm(node);
	};
	return request;
}

vector<DatabaseRequestHolder> ReadDatabaseRequests(const Json::Node& node_) {
	const vector<Json::Node>& nodeRequests = node_.AsArray();
	const size_t request_count = nodeRequests.size();

	vector<DatabaseRequestHolder> requests;
	requests.reserve(request_count);

	for (const auto& node : nodeRequests) {
		if (auto request = ParseDatabaseRequest(node)) {
			requests.push_back(move(request));
		}
	}

	return requests;
}

vector<string> ProcessDatabaseRequests(DirectoryTransport& directory, const vector<DatabaseRequestHolder>& requests) {
	vector<string> responses;
	for (const auto& request_holder : requests) {
		if (request_holder->type == DatabaseRequest::Type::BUS) {
			const auto& request = dynamic_cast<const ComputeBusDatabaseRequest&>(*request_holder);
			responses.push_back(move(request.Process(directory)));
		}
		else if (request_holder->type == DatabaseRequest::Type::STOP) {
			const auto& request = dynamic_cast<const ComputeStopDatabaseRequest&>(*request_holder);
			responses.push_back(move(request.Process(directory)));
		}
		else if (request_holder->type == DatabaseRequest::Type::ROUTE) {
			const auto& request = dynamic_cast<const ComputeRouteDatabaseRequest&>(*request_holder);
			responses.push_back(move(request.Process(directory)));
		}
	}
	return responses;
}

void PrintResponses(const vector<string>& responses, ostream& stream = cout) {
	stream << "[";
	bool first = true;
	for (const string& response : responses) {
		if (first) {
			stream << response;
			first = false;
		}
		else {
			stream << ", ";
			stream << response;
		}
	}
	stream << "]";
}