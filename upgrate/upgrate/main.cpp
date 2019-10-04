#ifdef _MSC_VER
#include "test_runner.h"
#endif

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
#include <fstream>

#include "json.h"
#include "graph.h"
#include "router.h"

#include "DirectoryTransport.h"
#include "Request.h"
#include "RequestCreate.h"
#include "RequestDatabase.h"

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


void ReadRoutingSettings(DirectoryTransport& directory, const Json::Node& node) {
	const map<string, Json::Node>& settings = node.AsMap();
	directory.SetWaitTime(settings.at("bus_wait_time").AsInt());
	directory.SetBusVelocity(settings.at("bus_velocity").AsInt());
}

#ifdef _MSC_VER

void Test(string input_json_file, string output_json_file, string my_output_json_file) {
	fstream ssInput(input_json_file);
	if (!ssInput.is_open()) {
		cout << "File input.txt is not open!" << endl;
		return;
	}

	stringstream ssOutput;

	Json::Document doc = Json::Load(ssInput);
	const map<string, Json::Node>& nodes = doc.GetRoot().AsMap();

	DirectoryTransport directory;
	if (nodes.count("routing_settings")) {
		ReadRoutingSettings(directory, nodes.at("routing_settings"));
	}
	if (nodes.count("base_requests")) {
		const auto create_requests = ReadCreateRequests(nodes.at("base_requests"));
		const auto create_responses = ProcessCreateRequests(directory, create_requests);
	}

	if (nodes.count("stat_requests")) {
		const auto database_requests = ReadDatabaseRequests(nodes.at("stat_requests"));
		const auto responses = ProcessDatabaseRequests(directory, database_requests);
		PrintResponses(responses, ssOutput);
	}

	fstream ideal(output_json_file);
	if (!ssInput.is_open()) {
		cout << "File ideal.txt is not open!" << endl;
		return;
	}

	string sIdeal;
	string tmp = "";
	while (getline(ideal, tmp)) {
		sIdeal += tmp;
	}

	string sOutput = ssOutput.str();

	fstream outputFile(my_output_json_file);
	outputFile << sOutput;

	ASSERT_EQUAL(sIdeal, sOutput);
}

void Test0() {
	Test("transport-test0-input.json",
		"transport-test0-output.json",
		"transport-test0-my-output.json");
}

void Test2() {
	Test("transport-test2-input.json",
		"transport-test2-output.json",
		"transport-test2-my-output.json");
}

void Test1() {
	fstream ssInput("transport-input_test1.json");
	if (!ssInput.is_open()) {
		cout << "File input.txt is not open!" << endl;
		return;
	}

	stringstream ssOutput;

	Json::Document doc = Json::Load(ssInput);
	const map<string, Json::Node>& nodes = doc.GetRoot().AsMap();

	DirectoryTransport directory;
	if (nodes.count("routing_settings")) {
		ReadRoutingSettings(directory, nodes.at("routing_settings"));
	}
	if (nodes.count("base_requests")) {
		const auto create_requests = ReadCreateRequests(nodes.at("base_requests"));
		const auto create_responses = ProcessCreateRequests(directory, create_requests);
	}

	if (nodes.count("stat_requests")) {
		const auto database_requests = ReadDatabaseRequests(nodes.at("stat_requests"));
		const auto responses = ProcessDatabaseRequests(directory, database_requests);
		PrintResponses(responses, ssOutput);
	}

	fstream ideal("transport-output_test1.json");
	if (!ssInput.is_open()) {
		cout << "File ideal.txt is not open!" << endl;
		return;
	}

	string sIdeal;
	string tmp = "";
	while (getline(ideal, tmp)) {
		sIdeal += tmp;
	}

	string sOutput = ssOutput.str();

	fstream outputFile("output.txt");
	outputFile << sOutput;

	ASSERT_EQUAL(sIdeal, sOutput);
}

void TestMain() {
	//stringstream ss2("{\"base_requests\": [{\"type\": \"Bus\", \"name\": \"256\", \"stops\": [\"Biryulyovo Zapadnoye\", \"Universam\", \"Biryulyovo Zapadnoye\"], \"is_roundtrip\": true}, {\"type\": \"Stop\", \"name\": \"Tolstopaltsevo\", \"latitude\": 55.611087, \"longitude\": 37.20829, \"road_distances\": {\"Marushkino\": 3900}}]}");
	//Json::Document doc2 = Json::Load(ss2);
	//
	//string sTmp;
	//string tmp1 = "";
	//while (getline(ss2, tmp1)) {
	//	sTmp += tmp1;
	//}

	
	
	fstream ssInput("transport-input4.json");
	if (!ssInput.is_open()) {
		cout << "File input.txt is not open!" << endl;
		return;
	}

	stringstream ssOutput;

	Json::Document doc = Json::Load(ssInput);
	const map<string, Json::Node>& nodes = doc.GetRoot().AsMap();

	DirectoryTransport directory;
	if (nodes.count("routing_settings")) {
		ReadRoutingSettings(directory, nodes.at("routing_settings"));
	}
	if (nodes.count("base_requests")) {
		const auto create_requests = ReadCreateRequests(nodes.at("base_requests"));
		const auto create_responses = ProcessCreateRequests(directory, create_requests);
	}

	if (nodes.count("stat_requests")) {
		const auto database_requests = ReadDatabaseRequests(nodes.at("stat_requests"));
		const auto responses = ProcessDatabaseRequests(directory, database_requests);
		PrintResponses(responses, ssOutput);
	}

	fstream ideal("transport-output4.json");
	if (!ssInput.is_open()) {
		cout << "File ideal.txt is not open!" << endl;
		return;
	}

	string sIdeal;
	string tmp = "";
	while (getline(ideal, tmp)) {
		sIdeal += tmp;
	}

	string sOutput = ssOutput.str();

	fstream outputFile("output.txt");
	outputFile << sOutput;

	ASSERT_EQUAL(sIdeal, sOutput);
}

#endif

int main() {
   // try {
#ifdef _MSC_VER
        TestRunner tr;
		RUN_TEST(tr, Test0);
		RUN_TEST(tr, Test2);
		//RUN_TEST(tr, Test1);
		//RUN_TEST(tr, TestMain);

        //system("pause");
#else

		Json::Document doc = Json::Load(cin);
		const map<string, Json::Node>& nodes = doc.GetRoot().AsMap();

		DirectoryTransport directory;
		ReadRoutingSettings(directory, node.at("routing_settings"));
		const auto create_requests = ReadCreateRequests(nodes.at("base_requests"));
		const auto create_responses = ProcessCreateRequests(directory, create_requests);
		const auto database_requests = ReadDatabaseRequests(nodes.at("stat_requests"));
		const auto responses = ProcessDatabaseRequests(directory, database_requests);
		PrintResponses(responses);
#endif
    //}
    //catch (exception& e) {
    //    cerr << "main catch: " << e.what() << endl;
    //    //throw;
    //}

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}