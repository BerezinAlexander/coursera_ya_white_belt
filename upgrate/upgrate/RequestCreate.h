#pragma once

#include <memory>
#include <unordered_map>

#include "json.h"
#include "DirectoryTransport.h"

using namespace std;

struct CreateRequest;
using CreateRequestHolder = unique_ptr<CreateRequest>;

struct CreateRequest {
	enum class Type {
		BUS,
		STOP
	};

	CreateRequest(Type type) : type(type) {}
	static CreateRequestHolder Create(Type type);
	virtual void ParseForm(const Json::Node& input) = 0;
	virtual ~CreateRequest() = default;

	const Type type;
};

const unordered_map<string_view, CreateRequest::Type> STR_TO_CREATE_REQUEST_TYPE = {
	{"Bus", CreateRequest::Type::BUS},
	{"Stop", CreateRequest::Type::STOP}
};

template <typename ResultType>
struct ReadRequest : CreateRequest {
	using CreateRequest::CreateRequest;
	virtual ResultType Process(const DirectoryTransport& directory) const = 0;
};

struct ModifyRequest : CreateRequest {
	using CreateRequest::CreateRequest;
	virtual void Process(DirectoryTransport& directory) const = 0;
};

struct ComputeBusCreateRequest : ModifyRequest {
	ComputeBusCreateRequest() : ModifyRequest(Type::BUS) {}

	void ParseForm(const Json::Node& input) override {
		const map<string, Json::Node>& m = input.AsMap();
		name = m.at("name").AsString();

		for (const auto& stop : m.at("stops").AsArray()) {
			stops.emplace_back(stop.AsString());
		}

		bool isRoundTrip = static_cast<bool>(m.at("is_roundtrip").AsBool());
		if (!isRoundTrip) {
			for (int i = stops.size() - 2; i >= 0; --i) {
				stops.emplace_back(stops[i]);
			}
		}
	}

	void Process(DirectoryTransport& directory) const override {
		directory.addBus(name, stops);
	}

	string name;
	vector<string> stops;
};

struct ComputeStopCreateRequest : ModifyRequest {
	ComputeStopCreateRequest() : ModifyRequest(Type::STOP) {}

	void ParseForm(const Json::Node& input) override {
		const map<string, Json::Node>& m = input.AsMap();
		name = m.at("name").AsString();
		latitude = m.at("latitude").AsDouble();
		longitude = m.at("longitude").AsDouble();

		for (const auto&[stop, dist] : m.at("road_distances").AsMap()) {
			distToStops.emplace_back(stop, dist.AsInt());
		}
	}

	void Process(DirectoryTransport& directory) const override {
		directory.addStop(name, latitude, longitude, distToStops);
	}

	string name;
	double latitude = 0;
	double longitude = 0;
	vector<pair<string, int>> distToStops;
};

CreateRequestHolder CreateRequest::Create(CreateRequest::Type type) {
	switch (type) {
	case CreateRequest::Type::BUS:
		return make_unique<ComputeBusCreateRequest>();
	case CreateRequest::Type::STOP:
		return make_unique<ComputeStopCreateRequest>();
	default:
		return nullptr;
	}
}

template <typename Number>
Number ReadNumberOnLine(istream& stream) {
	Number number;
	stream >> number;
	string dummy;
	getline(stream, dummy);
	return number;
}

optional<CreateRequest::Type> ConvertCreateRequestTypeFromString(string_view type_str) {
	if (const auto it = STR_TO_CREATE_REQUEST_TYPE.find(type_str);
		it != STR_TO_CREATE_REQUEST_TYPE.end()) {
		return it->second;
	}
	else {
		return nullopt;
	}
}

CreateRequestHolder ParseCreateRequest(const Json::Node& node_) {
	const map<string, Json::Node>& node = node_.AsMap();
	const auto request_type = ConvertCreateRequestTypeFromString(node.at("type").AsString());
	if (!request_type) {
		return nullptr;
	}
	CreateRequestHolder request = CreateRequest::Create(*request_type);
	if (request) {
		request->ParseForm(node);
	};
	return request;
}

vector<CreateRequestHolder> ReadCreateRequests(const Json::Node& node_) {
	const vector<Json::Node> nodeRequests = node_.AsArray();
	const size_t request_count = nodeRequests.size();

	vector<CreateRequestHolder> requests;
	requests.reserve(request_count);

	for (const auto& node : nodeRequests) {
		if (auto request = ParseCreateRequest(node)) {
			requests.push_back(move(request));
		}
	}
	return requests;
}

vector<double> ProcessCreateRequests(DirectoryTransport& directory, const vector<CreateRequestHolder>& requests) {
	vector<double> responses;
	for (const auto& request_holder : requests) {
		const auto& request = static_cast<const ModifyRequest&>(*request_holder);
		request.Process(directory);


		//if (request_holder->type == Request::Type::COMPUTE_INCOME) {
		//    const auto& request = static_cast<const ComputeIncomeRequest&>(*request_holder);
		//    responses.push_back(request.Process(manager));
		//}
		//else {
		//    const auto& request = static_cast<const ModifyRequest&>(*request_holder);
		//    request.Process(manager);
		//}
	}
	return responses;
}

void PrintResponses(const vector<double>& responses, ostream& stream = cout) {
	//for (const double response : responses) {
	//    stream << response << endl;
	//}
}