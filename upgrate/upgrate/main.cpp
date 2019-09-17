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
    if (pos != s.length()) {
        std::stringstream error;
        error << "string " << s << " contains " << (s.length() - pos) << " trailing chars";
        cerr << "string: " << s << endl;
        cerr << error.str() << endl;
        throw invalid_argument(error.str());
    }
    return result;
}

double ConvertToDouble(string_view s) {
    size_t pos;
    const double result = stof(string(s), &pos);
    if (pos != s.length()) {
        std::stringstream error;
        error << "string " << s << " contains " << (s.length() - pos) << " trailing chars";
        throw invalid_argument(error.str());
    }
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

int ReadBusNumber(string_view& s) {
    auto lhs = ReadToken(s, ": ");
    int busNumber = ConvertToInt(lhs);
    
    return busNumber;
}

int ReadDatabaseBusNumber(string_view& s) {
    int busNumber = ConvertToInt(s);
    return busNumber;
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

string ReadStopName(string_view& s) {
    auto lhs = ReadToken(s, ": ");
    return string(lhs);
}

double ReadLatitude(string_view& s) {
    auto lhs = ReadToken(s, ", ");
    double value = ConvertToDouble(lhs);
    return value;
}

double ReadLongitude(string_view& s) {
    double value = ConvertToDouble(s);
    return value;
}

/*
    //
*/

struct BusInfo {
    int busNumber;
    int stopCount;
    int uniqueStopCount;
    double lengthRoute;
};

class DirectoryTransport {
public:
    void addBus(int number, const vector<string>& stops) {
        buses[number] = stops;
    }

    void addStop(const string& name, double lat, double lon) {
        stops[name] = {lat, lon};
    }

    BusInfo CalcBusInfo(int number) const {
        if (!buses.count(number))
            return {number,0,0,0};

        if (busesInfoCache.count(number))
            return busesInfoCache.at(number);

        const auto& busStops = buses.at(number);

        BusInfo info;
        info.busNumber = number;
        info.stopCount = calcStopCount(busStops);
        info.uniqueStopCount = calcUniqueStopCount(busStops);
        info.lengthRoute = calcRouteLength(busStops);

        busesInfoCache[number] = info;

        return info;
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
            const auto[lat1, lon1] = stops.at(*itFirst);
            const auto[lat2, lon2] = stops.at(*itSecond);
            double dist = distance( gradToRad(lat1), gradToRad(lon1),
                                    gradToRad(lat2), gradToRad(lon2));
            totalLength += dist;

            itFirst = itSecond;
            if (++itSecond == stops_.end()) {
                break;
            }
        }

        return totalLength;
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
    unordered_map<int, vector<string>> buses;
    unordered_map<string, pair<double, double>> stops;
    mutable unordered_map<int, BusInfo> busesInfoCache;
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
        number = ReadBusNumber(input);
        stops = move(ReadBusStops(input));
    }

    void Process(DirectoryTransport& directory) const override {
        directory.addBus(number, stops);
    }

    int number = 0;
    vector<string> stops;
};

struct ComputeStopCreateRequest : ModifyRequest {
    ComputeStopCreateRequest() : ModifyRequest(Type::STOP) {}

    void ParseForm(string_view input) override {
        name = ReadStopName(input);
        latitude = ReadLatitude(input);
        longitude = ReadLongitude(input);
    }

    void Process(DirectoryTransport& directory) const override {
        directory.addStop(name, latitude, longitude);
    }

    string name = "";
    double latitude = 0;
    double longitude = 0;
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
        BUS
    };

    DatabaseRequest(Type type) : type(type) {}
    static DatabaseRequestHolder Create(Type type);
    virtual void ParseForm(string_view input) = 0;
    virtual ~DatabaseRequest() = default;

    const Type type;
};

const unordered_map<string_view, DatabaseRequest::Type> STR_TO_DATABASE_REQUEST_TYPE = {
    {"Bus", DatabaseRequest::Type::BUS}
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

struct ComputeBusDatabaseRequest : ReadDatabaseRequest<BusInfo> {
    ComputeBusDatabaseRequest() : ReadDatabaseRequest<BusInfo>(Type::BUS) {}

    void ParseForm(string_view input) override {
        number = ReadDatabaseBusNumber(input);
    }

    BusInfo Process(DirectoryTransport& directory) const override {
        return directory.CalcBusInfo(number);
    }

    int number = 0;
};

DatabaseRequestHolder DatabaseRequest::Create(DatabaseRequest::Type type) {
    switch (type) {
    case DatabaseRequest::Type::BUS:
        return make_unique<ComputeBusDatabaseRequest>();
    //case DatabaseRequest::Type::STOP:
    //    return make_unique<ComputeStopDatabaseRequest>();
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

vector<BusInfo> ProcessDatabaseRequests(DirectoryTransport& directory, const vector<DatabaseRequestHolder>& requests) {
    vector<BusInfo> responses;
    for (const auto& request_holder : requests) {
        const auto& request = dynamic_cast<const ComputeBusDatabaseRequest&>(*request_holder);
        responses.push_back(request.Process(directory));


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

void PrintResponses(const vector<BusInfo>& responses, ostream& stream = cout) {
    for (const BusInfo& response : responses) {
        if (response.stopCount == 0) {
            stream << "Bus " << response.busNumber << ": not found" << endl;
        }
        else {
            stream << "Bus " << response.busNumber << ": "
                << response.stopCount << " stops on route, "
                << response.uniqueStopCount << " unique stops, "
                << setprecision(6) << response.lengthRoute << " route length" << endl;
        }
    }
}


int main() {
    DirectoryTransport directory;
    const auto create_requests = ReadCreateRequests();
    const auto create_responses = ProcessCreateRequests(directory, create_requests);
    const auto database_requests = ReadDatabaseRequests();
    const auto responses = ProcessDatabaseRequests(directory, database_requests);
    PrintResponses(responses);

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}