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

// информация по автобусу
struct BusInfo {
	string busNumber;
	int stopCount;
	int uniqueStopCount;
	double lengthRoute;
	double curvature;
};

// Транспортный справочник
class DirectoryTransport {
public:
	// типы участков
	enum class RouteItemType {
		BUS,
		WAIT
	};

	// участок маршрута
	struct ItemRoute {
		RouteItemType type; 
		double time;
		string name; // маршрута или остановки
		int count; // количество остановок, которые нужно проехать (только для автобуса)
	};

	// информация по маршруту
	struct RouteResult {
		bool isCorrect;
		double total_time;
		vector<ItemRoute> items;
	};

	DirectoryTransport() : graph(0) {}
	
	// добавление автобуса
	void addBus(string_view number, const vector<string>& stops);
	
	// добавление остановки
	void addStop(string_view name, double lat, double lon, const vector<pair<string, int>>& distToStop);
	
	// получить информацию по автобусу
	BusInfo CalcBusInfo(string_view number) const;
	
	// получить информацию по остановке
	pair<bool, set<string>> CalcStopInfo(string_view name) const;
	
	// установить время ожидания автобуса
	void SetWaitTime(int waitTime_);

	// установить скорость автобуса
	void SetBusVelocity(int v);

	// рассчитать оптимальный маршрут
	RouteResult CalcRoute(string_view from, string_view to);

private:
	// количество остановок на маршруте автобуса
	int calcStopCount(const vector<string>& stops_) const;

	// количество уникальных остановок на маршруте автобуса
	int calcUniqueStopCount(const vector<string>& stops_) const;

	// длина маршрута
	double calcRouteLength(const vector<string>& stops_) const;

	// длина маршрута без учета кривизны
	double calcRouteLengthGeo(const vector<string>& stops_) const;

	// кривизна маршрута
	double curvatureRoute(double realDist, double geoDist);

	// градусы в радианы
	double gradToRad(double grad) const;

	// дистанция между двумя географическими точками
	double distance(double lat1, double lon1, double lat2, double lon2) const;

	// 
	float calcTime(int distance);

	// создание маршрутизатора
	void createRouter();

	// формирование вершин
	void formationVertexes();

	int getDistanceBetweenTwoNearStops(const string& stop1, const string& stop2);

private:
	unordered_map<string, vector<string>> buses;	// автобусы и их остановки
	unordered_map<string, pair<double, double>> stops; // остановки и их координаты
	unordered_map<string, set<string>> stopBuses; // остановки и их автобусы
	mutable unordered_map<string, BusInfo> busesInfoCache; // кешированная информация по автобусам
	unordered_map<string, unordered_map<string, int>> distanceBetweenStops; // дистанция между остановками
	int waitTime; // время ожидания автобуса
	int busVelocity; // скорость автобусов
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