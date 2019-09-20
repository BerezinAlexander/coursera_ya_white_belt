#include "test_runner.h"

#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <optional>
#include <sstream>
#include <set>
#include <math.h>
#include <iomanip>
#include <vector>

using namespace std;

int ConvertToInt(string_view s) {
    size_t pos;
    const int result = stoi(string(s), &pos);
    //if (pos != s.length()) {
    //    std::stringstream error;
    //    error << "string " << s << " contains " << (s.length() - pos) << " trailing chars";
    //    cerr << "string: " << s << endl;
    //    cerr << error.str() << endl;
    //    throw invalid_argument(error.str());
    //}
    return result;
}

double ConvertToDouble(string_view s) {
    size_t pos;
    const double result = stof(string(s), &pos);
    //if (pos != s.length()) {
    //    std::stringstream error;
    //    error << "string " << s << " contains " << (s.length() - pos) << " trailing chars";
    //    throw invalid_argument(error.str());
    //}
    return result;
}

pair<string_view, optional<string_view>> SplitToStrict(string_view s, string_view delimeter = " ") {
    const size_t pos = s.find(delimeter);
    if (pos == s.npos) {
        return { s, nullopt };
    }
    else {
        return { s.substr(0, pos), s.substr(pos + delimeter.length()) };
    }
}

pair<string_view, string_view> SplitTwo(string_view s, string_view delimeter = " ") {
    const auto[lhs, rhs_opt] = SplitToStrict(s, delimeter);
    return { lhs, rhs_opt.value_or("") };
}

string_view ReadToken(string_view& s, string_view delimiter = " ") {
    const auto[lhs, rhs] = SplitTwo(s, delimiter);
    s = rhs;
    return lhs;
}

string_view ReadBusNumber(string_view& s) {
    auto lhs = ReadToken(s, ": "); 
    return lhs;
}

string_view ReadDatabaseBusNumber(string_view& s) {
    //int busNumber = ConvertToInt(s);
    return s;
}

vector<string> ReadBusStops(string_view s) {
    bool loop = false;
    string delimeter = " - ";
    size_t pos = s.find("-");
    if (pos == s.npos) {
        loop = true;
        delimeter = " > ";
    }

    vector<string> stops;
    stops.reserve(100);
    while (true) {
        string_view lhs = ReadToken(s, delimeter);
        stops.emplace_back(lhs);
        if (s.empty())
            break;
    }

    if (!loop) {
        for (int i = stops.size() - 2; i >= 0; --i) {
            stops.emplace_back(stops[i]);
        }
    }
    
    return stops;
}

string_view ReadStopName(string_view& s) {
    auto lhs = ReadToken(s, ": ");
    return lhs;
}

double ReadLatitude(string_view& s) {
    auto lhs = ReadToken(s, ", ");
    double value = ConvertToDouble(lhs);
    return value;
}

double ReadLongitude(string_view& s) {
    double value = ConvertToDouble(ReadToken(s));
    return value;
}

pair<string_view, int> ReadDistToStop(string_view s) {
	auto dist = ReadToken(s, "m to ");
	auto stop = s;
	return { s, stoi(string(dist))};
}

vector<pair<string, int>> ReadDistanceToStops(string_view& s) {
	vector<pair<string, int>> distToStops;
	while (!s.empty()) {
		auto lhs = ReadToken(s, ", ");
		distToStops.emplace_back(ReadDistToStop(lhs));
	}
	return distToStops;
}

/*
    //
*/

struct BusInfo {
    string busNumber;
    int stopCount;
    int uniqueStopCount;
    double lengthRoute;
	double curvature;
};

class DirectoryTransport {
public:
    void addBus(string_view number, const vector<string>& stops) {
        buses[string(number)] = stops;
        for (const auto& stop : stops) {
            stopBuses[stop].insert(number);
        }
    }

    void addStop(string_view name, double lat, double lon, const vector<pair<string, int>>& distToStop) {
        stops[string(name)] = {lat, lon};
        stopBuses[name];

		for (auto it = distToStop.begin(); it != distToStop.end(); ++it) {
			distanceBetweenStops[name][it->first] = it->second;
			
			if (distanceBetweenStops.count(it->first) != 0) {
				if (distanceBetweenStops[it->first].count(name) == 0) {
					distanceBetweenStops[it->first][name] = it->second;
				}
			}
			else {
				distanceBetweenStops[it->first][name] = it->second;
			}
		}
    }

    BusInfo CalcBusInfo(string_view number) const {
        if (!buses.count(string(number)))
            return {string(number),0,0,0};

        if (busesInfoCache.count(string(number)))
            return busesInfoCache.at(string(number));

        const auto& busStops = buses.at(string(number));

        BusInfo info;
        info.busNumber = number;
        info.stopCount = calcStopCount(busStops);
        info.uniqueStopCount = calcUniqueStopCount(busStops);
        info.lengthRoute = calcRouteLength(busStops);
		info.curvature = info.lengthRoute / calcRouteLengthGeo(busStops);

        busesInfoCache[string(number)] = info;

        return info;
    }

    pair<bool, set<string_view>> CalcStopInfo(string_view name) const {
        if (!stopBuses.count(name))
            return { false, set<string_view>() };

        return { true, stopBuses.at(name) };
    }

private:
    int calcStopCount(const vector<string>& stops_) const {
        return stops_.size();
    }

    int calcUniqueStopCount(const vector<string>& stops_) const {
        set<string_view> uniq(stops_.begin(), stops_.end());
        return uniq.size();
    }

    double calcRouteLength(const vector<string>& stops_) const {
        if (stops_.size() < 2)
            return 0;

        auto itFirst = stops_.begin();
        auto itSecond = next(itFirst);

        double totalLength = 0;

        while (itSecond != stops_.end()) {
			double dist = distanceBetweenStops.at(*itFirst).at(*itSecond);
			totalLength += dist;

            itFirst = itSecond;
            if (++itSecond == stops_.end()) {
                break;
            }
        }

        return totalLength;
    }

	double calcRouteLengthGeo(const vector<string>& stops_) const {
		if (stops_.size() < 2)
			return 0;

		auto itFirst = stops_.begin();
		auto itSecond = next(itFirst);

		double totalLength = 0;

		while (itSecond != stops_.end()) {
			const auto[lat1, lon1] = stops.at(*itFirst);
			const auto[lat2, lon2] = stops.at(*itSecond);
			double dist = distance(gradToRad(lat1), gradToRad(lon1),
				gradToRad(lat2), gradToRad(lon2));
			totalLength += dist;

			itFirst = itSecond;
			if (++itSecond == stops_.end()) {
				break;
			}
		}

		return totalLength;
	}

	double curvatureRoute(double realDist, double geoDist) {
		return realDist / geoDist;
	}

    double gradToRad(double grad) const {
        const double pi = 3.1415926535;
        return grad * pi / 180.;
    }

    double distance(double lat1, double lon1, double lat2, double lon2) const {
        const double radiusEarth = 6371000.;
        return acos( sin(lat1) * sin(lat2) 
                     + cos(lat1) * cos(lat2) * cos(abs(lon1 - lon2))
                   ) * radiusEarth;
    }

private:
    unordered_map<string, vector<string>> buses;
    unordered_map<string, pair<double, double>> stops;
    unordered_map<string_view, set<string_view>> stopBuses;
    mutable unordered_map<string, BusInfo> busesInfoCache;
    unordered_map<string_view, unordered_map<string_view, int>> distanceBetweenStops;
};

//struct Request {
//    enum class Type {
//        BUS,
//        STOP,
//        BUS_INFO
//    };
//    virtual void ParseForm(string_view input) = 0;
//    virtual ~Request() = default;
//};

struct CreateRequest;
using CreateRequestHolder = unique_ptr<CreateRequest>;

struct CreateRequest {
    enum class Type {
        BUS,
        STOP
    };

    CreateRequest(Type type) : type(type) {}
    static CreateRequestHolder Create(Type type);
    virtual void ParseForm(string_view input) = 0;
    virtual ~CreateRequest() = default;

    const Type type;
};

//struct DatabaseRequest;
//using DatabaseRequestHolder = unique_ptr<DatabaseRequest>;
//
//struct DatabaseRequest : Request {
//    enum class Type {
//        BUS
//    };
//
//    DatabaseRequest(Type type) : type(type) {}
//    static DatabaseRequestHolder Create(Type type);
//
//    const Type type;
//};

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

    void ParseForm(string_view input) override {
        string_view tmp = input;
        try{
            number = ReadBusNumber(input);
            stops = move(ReadBusStops(input));
        }
        catch (...) {
            cerr << "Exception: BusCreate is failed. string = [" << tmp << "]" << endl;
            throw;
        }
    }

    void Process(DirectoryTransport& directory) const override {
        directory.addBus(number, stops);
    }

    string number;
    vector<string> stops;
};

struct ComputeStopCreateRequest : ModifyRequest {
    ComputeStopCreateRequest() : ModifyRequest(Type::STOP) {}

    void ParseForm(string_view input) override {
        try{
            name = ReadStopName(input);
            latitude = ReadLatitude(input);
            longitude = ReadLongitude(input);
			distToStops = ReadDistanceToStops(input);
        }
        catch (...) {
            cerr << "Exception: StopCreate is failed" << endl;
            throw;
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

//struct BusInfo {
//    int stopCount;
//    int uniqueStopCount;
//    double lengthRoute;
//};
//
//struct ComputeBusDatabaseRequest : ReadRequest<BusInfo> {
//    ComputeBusDatabaseRequest() : ModifyRequest(Type::BUS_INFO) {}
//
//    void ParseForm(string_view input) override {
//        number = ReadBusNumber(input);
//    }
//
//    void Process(DirectoryTransport& directory) const override {
//        directory.addBus(number, stops);
//    }
//
//    int number = 0;
//};

CreateRequestHolder CreateRequest::Create(CreateRequest::Type type) {
    switch (type) {
    case CreateRequest::Type::BUS:
        return make_unique<ComputeBusCreateRequest>();
    case CreateRequest::Type::STOP:
        return make_unique<ComputeStopCreateRequest>();
    //case Request::Type::SPEND:
    //    return make_unique<AddMoneyRequest<-1>>();
    //case Request::Type::PAY_TAX:
    //    return make_unique<PayTaxRequest>();
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

CreateRequestHolder ParseCreateRequest(string_view request_str) {
    const auto request_type = ConvertCreateRequestTypeFromString(ReadToken(request_str));
    if (!request_type) {
        return nullptr;
    }
    CreateRequestHolder request = CreateRequest::Create(*request_type);
    if (request) {
        request->ParseForm(request_str);
    };
    return request;
}

vector<CreateRequestHolder> ReadCreateRequests(istream& in_stream = cin) {
    const size_t request_count = ReadNumberOnLine<size_t>(in_stream);

    vector<CreateRequestHolder> requests;
    requests.reserve(request_count);

    for (size_t i = 0; i < request_count; ++i) {
        string request_str;
        getline(in_stream, request_str);
        if (auto request = ParseCreateRequest(request_str)) {
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

/*

*/

struct DatabaseRequest;
using DatabaseRequestHolder = unique_ptr<DatabaseRequest>;

struct DatabaseRequest {
    enum class Type {
        BUS,
        STOP
    };

    DatabaseRequest(Type type) : type(type) {}
    static DatabaseRequestHolder Create(Type type);
    virtual void ParseForm(string_view input) = 0;
    virtual ~DatabaseRequest() = default;

    const Type type;
};

const unordered_map<string_view, DatabaseRequest::Type> STR_TO_DATABASE_REQUEST_TYPE = {
    {"Bus", DatabaseRequest::Type::BUS},
    {"Stop", DatabaseRequest::Type::STOP}
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

    void ParseForm(string_view input) override {
        try {
            number = ReadDatabaseBusNumber(input);
        }
        catch (...) {
            cerr << "Exception: ReadDatabaseBusNumber is failed" << endl;
            throw;
        }
    }

    string Process(DirectoryTransport& directory) const override {
        BusInfo info = directory.CalcBusInfo(number);
        stringstream stream;
        if (info.stopCount == 0) {
            stream << "Bus " << info.busNumber << ": not found" << endl;
        }
        else {
            stream << "Bus " << info.busNumber << ": "
                << info.stopCount << " stops on route, "
                << info.uniqueStopCount << " unique stops, "
                << setprecision(6) << info.lengthRoute << " route length, " 
				<< setprecision(6) << info.curvature << " curvature" << endl;
        }
        return stream.str();
    }

    string number;
};

struct ComputeStopDatabaseRequest : ReadDatabaseRequest<string> {
    ComputeStopDatabaseRequest() : ReadDatabaseRequest<string>(Type::STOP) {}

    void ParseForm(string_view input) override {
        try {
            name = ReadDatabaseBusNumber(input);
        }
        catch (...) {
            cerr << "Exception: ReadDatabaseBusNumber is failed" << endl;
            throw;
        }
    }

    string Process(DirectoryTransport& directory) const override {
        auto[isFound, buses] = directory.CalcStopInfo(name);
        stringstream stream;
        if (!isFound) {
            stream << "Stop " << name << ": not found" << endl;
        }
        else if (buses.empty()) {
            stream << "Stop " << name << ": no buses" << endl;
        }
        else {
            stream << "Stop " << name << ": buses";
            for (auto& bus : buses) {
                stream << " " << bus;
            }
            stream << endl;
        }

        return stream.str();
    }

    string name;
};

DatabaseRequestHolder DatabaseRequest::Create(DatabaseRequest::Type type) {
    switch (type) {
    case DatabaseRequest::Type::BUS:
        return make_unique<ComputeBusDatabaseRequest>();
    case DatabaseRequest::Type::STOP:
        return make_unique<ComputeStopDatabaseRequest>();
        //case Request::Type::SPEND:
        //    return make_unique<AddMoneyRequest<-1>>();
        //case Request::Type::PAY_TAX:
        //    return make_unique<PayTaxRequest>();
    default:
        return nullptr;
    }
}

//template <typename Number>
//Number ReadNumberOnLine(istream& stream) {
//    Number number;
//    stream >> number;
//    string dummy;
//    getline(stream, dummy);
//    return number;
//}

optional<DatabaseRequest::Type> ConvertDatabaseRequestTypeFromString(string_view type_str) {
    if (const auto it = STR_TO_DATABASE_REQUEST_TYPE.find(type_str);
        it != STR_TO_DATABASE_REQUEST_TYPE.end()) {
        return it->second;
    }
    else {
        return nullopt;
    }
}

DatabaseRequestHolder ParseDatabaseRequest(string_view request_str) {
    const auto request_type = ConvertDatabaseRequestTypeFromString(ReadToken(request_str));
    if (!request_type) {
        return nullptr;
    }
    DatabaseRequestHolder request = DatabaseRequest::Create(*request_type);
    if (request) {
        request->ParseForm(request_str);
    };
    return request;
}

vector<DatabaseRequestHolder> ReadDatabaseRequests(istream& in_stream = cin) {
    const size_t request_count = ReadNumberOnLine<size_t>(in_stream);

    vector<DatabaseRequestHolder> requests;
    requests.reserve(request_count);

    for (size_t i = 0; i < request_count; ++i) {
        string request_str;
        getline(in_stream, request_str);
        if (auto request = ParseDatabaseRequest(request_str)) {
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

void PrintResponses(const vector<string>& responses, ostream& stream = cout) {
    for (const string& response : responses) {
        stream << response;
        //if (response.stopCount == 0) {
        //    stream << "Bus " << response.busNumber << ": not found" << endl;
        //}
        //else {
        //    stream << "Bus " << response.busNumber << ": "
        //        << response.stopCount << " stops on route, "
        //        << response.uniqueStopCount << " unique stops, "
        //        << setprecision(6) << response.lengthRoute << " route length" << endl;
        //}
    }
}

void TestMain() {
    string sInput = "\
10\n\
Stop Tolstopaltsevo: 55.611087, 37.20829\n\
Stop Marushkino: 55.595884, 37.209755\n\
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n\
Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n\
Stop Rasskazovka: 55.632761, 37.333324\n\
Stop Biryulyovo Zapadnoye: 55.574371, 37.6517\n\
Stop Biryusinka: 55.581065, 37.64839\n\
Stop Universam: 55.587655, 37.645687\n\
Stop Biryulyovo Tovarnaya: 55.592028, 37.653656\n\
Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n\
3\n\
Bus 256\n\
Bus 750\n\
Bus 751\n\
";
    stringstream ssInput(sInput);

    stringstream ssOutput;

    DirectoryTransport directory;
    const auto create_requests = ReadCreateRequests(ssInput);
    const auto create_responses = ProcessCreateRequests(directory, create_requests);
    const auto database_requests = ReadDatabaseRequests(ssInput);
    const auto responses = ProcessDatabaseRequests(directory, database_requests);
    PrintResponses(responses, ssOutput);

    string sIdeal = "\
Bus 256: 6 stops on route, 5 unique stops, 4371.26 route length\n\
Bus 750: 5 stops on route, 3 unique stops, 20940 route length\n\
Bus 751: not found\n\
";
    string sOutput = ssOutput.str();
    ASSERT_EQUAL(sIdeal, sOutput);
}

void Test1() {
    string sInput = "\
3\n\
Stop Tolstopaltsevo: 55.611087, 37.20829\n\
Stop Marushkino: 55.595884, 37.209755\n\
Bus 750: Tolstopaltsevo > Marushkino\n\
1\n\
Bus 750\n\
";
    stringstream ssInput(sInput);

    stringstream ssOutput;

    DirectoryTransport directory;
    const auto create_requests = ReadCreateRequests(ssInput);
    const auto create_responses = ProcessCreateRequests(directory, create_requests);
    const auto database_requests = ReadDatabaseRequests(ssInput);
    const auto responses = ProcessDatabaseRequests(directory, database_requests);
    PrintResponses(responses, ssOutput);

    string sIdeal = "\
Bus 750: 2 stops on route, 2 unique stops, 1693.26 route length\n\
";
    ASSERT_EQUAL(sIdeal, ssOutput.str());
}

void TestMain2() {
    string sInput = "\
13\n\
Stop Tolstopaltsevo: 55.611087, 37.20829\n\
Stop Marushkino: 55.595884, 37.209755\n\
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n\
Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n\
Stop Rasskazovka: 55.632761, 37.333324\n\
Stop Biryulyovo Zapadnoye: 55.574371, 37.6517\n\
Stop Biryusinka: 55.581065, 37.64839\n\
Stop Universam: 55.587655, 37.645687\n\
Stop Biryulyovo Tovarnaya: 55.592028, 37.653656\n\
Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n\
Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\n\
Stop Rossoshanskaya ulitsa: 55.595579, 37.605757\n\
Stop Prazhskaya: 55.611678, 37.603831\n\
6\n\
Bus 256\n\
Bus 750\n\
Bus 751\n\
Stop Samara\n\
Stop Prazhskaya\n\
Stop Biryulyovo Zapadnoye\n\
";
    stringstream ssInput(sInput);

    stringstream ssOutput;

    DirectoryTransport directory;
    const auto create_requests = ReadCreateRequests(ssInput);
    const auto create_responses = ProcessCreateRequests(directory, create_requests);
    const auto database_requests = ReadDatabaseRequests(ssInput);
    const auto responses = ProcessDatabaseRequests(directory, database_requests);
    PrintResponses(responses, ssOutput);

    string sIdeal = "\
Bus 256: 6 stops on route, 5 unique stops, 4371.26 route length\n\
Bus 750: 5 stops on route, 3 unique stops, 20940 route length\n\
Bus 751: not found\n\
Stop Samara: not found\n\
Stop Prazhskaya: no buses\n\
Stop Biryulyovo Zapadnoye: buses 256 828\n\
";
    string sOutput = ssOutput.str();
    ASSERT_EQUAL(sIdeal, sOutput);
}

void TestMain3() {
	string sInput = "\
13\n\
Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino\n\
Stop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka\n\
Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n\
Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n\
Stop Rasskazovka: 55.632761, 37.333324\n\
Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam\n\
Stop Biryusinka: 55.581065, 37.64839, 750m to Universam\n\
Stop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya\n\
Stop Biryulyovo Tovarnaya: 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya\n\
Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye\n\
Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\n\
Stop Rossoshanskaya ulitsa: 55.595579, 37.605757\n\
Stop Prazhskaya: 55.611678, 37.603831\n\
6\n\
Bus 256\n\
Bus 750\n\
Bus 751\n\
Stop Samara\n\
Stop Prazhskaya\n\
Stop Biryulyovo Zapadnoye\n\
";
	stringstream ssInput(sInput);

	stringstream ssOutput;

	DirectoryTransport directory;
	const auto create_requests = ReadCreateRequests(ssInput);
	const auto create_responses = ProcessCreateRequests(directory, create_requests);
	const auto database_requests = ReadDatabaseRequests(ssInput);
	const auto responses = ProcessDatabaseRequests(directory, database_requests);
	PrintResponses(responses, ssOutput);

	string sIdeal = "\
Bus 256: 6 stops on route, 5 unique stops, 5950 route length, 1.361239 curvature\n\
Bus 750: 5 stops on route, 3 unique stops, 27600 route length, 1.318084 curvature\n\
Bus 751: not found\n\
Stop Samara: not found\n\
Stop Prazhskaya: no buses\n\
Stop Biryulyovo Zapadnoye: buses 256 828\n\
";
	string sOutput = ssOutput.str();
	ASSERT_EQUAL(sIdeal, sOutput);
}

int main() {
    try {
#ifdef _MSC_VER
        TestRunner tr;
        //RUN_TEST(tr, TestMain);
        RUN_TEST(tr, Test1);
		//RUN_TEST(tr, TestMain2);
		RUN_TEST(tr, TestMain3);

        //system("pause");
#else
        DirectoryTransport directory;
        const auto create_requests = ReadCreateRequests();
        const auto create_responses = ProcessCreateRequests(directory, create_requests);
        const auto database_requests = ReadDatabaseRequests();
        const auto responses = ProcessDatabaseRequests(directory, database_requests);
        PrintResponses(responses);
#endif
    }
    catch (exception& e) {
        cerr << "main catch: " << e.what() << endl;
        //throw;
    }

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}