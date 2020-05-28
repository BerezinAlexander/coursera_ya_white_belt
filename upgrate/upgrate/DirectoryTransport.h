#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <set>
#include <memory>

#include "graph.h"
#include "router.h"

using namespace std;

// ���������� �� ��������
struct BusInfo {
	string busNumber;
	int stopCount;
	int uniqueStopCount;
	double lengthRoute;
	double curvature;
};

// ������������ ����������
class DirectoryTransport {
public:
	// ���� ��������
	enum class RouteItemType {
		BUS,
		WAIT
	};

	// ������� ��������
	struct ItemRoute {
		RouteItemType type; 
		double time;
		string name; // �������� ��� ���������
		int count; // ���������� ���������, ������� ����� �������� (������ ��� ��������)
	};

	// ���������� �� ��������
	struct RouteResult {
		bool isCorrect;
		double total_time;
		vector<ItemRoute> items;
	};

	DirectoryTransport() : graph(0) {}
	
	// ���������� ��������
	void addBus(string_view number, const vector<string>& stops);
	
	// ���������� ���������
	void addStop(string_view name, double lat, double lon, const vector<pair<string, int>>& distToStop);
	
	// �������� ���������� �� ��������
	BusInfo CalcBusInfo(string_view number) const;
	
	// �������� ���������� �� ���������
	pair<bool, set<string>> CalcStopInfo(string_view name) const;
	
	// ���������� ����� �������� ��������
	void SetWaitTime(int waitTime_);

	// ���������� �������� ��������
	void SetBusVelocity(int v);

	// ���������� ����������� �������
	RouteResult CalcRoute(string_view from, string_view to);

private:
	// ���������� ��������� �� �������� ��������
	int calcStopCount(const vector<string>& stops_) const;

	// ���������� ���������� ��������� �� �������� ��������
	int calcUniqueStopCount(const vector<string>& stops_) const;

	// ����� ��������
	double calcRouteLength(const vector<string>& stops_) const;

	// ����� �������� ��� ����� ��������
	double calcRouteLengthGeo(const vector<string>& stops_) const;

	// �������� ��������
	double curvatureRoute(double realDist, double geoDist);

	// ������� � �������
	double gradToRad(double grad) const;

	// ��������� ����� ����� ��������������� �������
	double distance(double lat1, double lon1, double lat2, double lon2) const;

	// 
	float calcTime(int distance);

	// �������� ��������������
	void createRouter();

	// ������������ ������
	void formationVertexes();

	int getDistanceBetweenTwoNearStops(const string& stop1, const string& stop2);

private:
	unordered_map<string, vector<string>> buses;	// �������� � �� ���������
	unordered_map<string, pair<double, double>> stops; // ��������� � �� ����������
	unordered_map<string, set<string>> stopBuses; // ��������� � �� ��������
	mutable unordered_map<string, BusInfo> busesInfoCache; // ������������ ���������� �� ���������
	unordered_map<string, unordered_map<string, int>> distanceBetweenStops; // ��������� ����� �����������
	int waitTime; // ����� �������� ��������
	int busVelocity; // �������� ���������
	unique_ptr<Graph::Router<float>> ptrRouter;
	Graph::DirectedWeightedGraph<float> graph;


	struct EdgesParam {
		RouteItemType type;
		Graph::Edge<float> edge;
		string name;
		int count_stops;
	};

	vector<EdgesParam> edgesIndex;
};