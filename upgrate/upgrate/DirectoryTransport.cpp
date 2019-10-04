#include <map>

#include "DirectoryTransport.h"

using namespace Graph;

ostream& operator << (ostream& os, const Graph::Edge<float>& edge) {
	os << "from " << edge.from << " to " << edge.to << " weight " << edge.weight;
	return os;
}

void DirectoryTransport::addBus(string_view number, const vector<string>& stops) {
	buses[string(number)] = stops;
	for (const auto& stop : stops) {
		stopBuses[stop].insert(string(number));
	}
}

void DirectoryTransport::addStop(string_view name, double lat, double lon, const vector<pair<string, int>>& distToStop) {
	stops[string(name)] = { lat, lon };
	stopBuses[string(name)];

	for (auto it = distToStop.begin(); it != distToStop.end(); ++it) {
		distanceBetweenStops[string(name)][it->first] = it->second;

		if (distanceBetweenStops.count(it->first) != 0) {
			if (distanceBetweenStops[it->first].count(string(name)) == 0) {
				distanceBetweenStops[it->first][string(name)] = it->second;
			}
		}
		else {
			distanceBetweenStops[it->first][string(name)] = it->second;
		}
	}
}

BusInfo DirectoryTransport::CalcBusInfo(string_view number) const {
	if (!buses.count(string(number)))
		return { string(number),0,0,0 };

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

pair<bool, set<string>> DirectoryTransport::CalcStopInfo(string_view name) const {
	if (!stopBuses.count(string(name)))
		return { false, set<string>() };

	return { true, stopBuses.at(string(name)) };
}

void DirectoryTransport::SetWaitTime(int waitTime_) {
	waitTime = waitTime_;
}

void DirectoryTransport::SetBusVelocity(int v) {
	busVelocity = v;
}

// рассчитать оптимальный маршрут

DirectoryTransport::RouteResult DirectoryTransport::CalcRoute(string_view from, string_view to) {
	using namespace Graph;

	createRouter();

	//size_t beginStop = m_stops[from] + STOPS_COUNT;
	//size_t endStop = m_stops[to];
	//std::optional<Router<float>::RouteInfo> opt_info = router.BuildRoute(beginStop, endStop);

	//RouteResult result;

	//if (opt_info.has_value()) {
	//	Router<float>::RouteInfo info = opt_info.value();
	//	result.total_time = info.weight;

	//	cout << endl;
	//	cout << "from " << from << " to " << to << " total_time " << info.weight << endl;

	//	vector<Edge<float>> route;
	//	for (int i = 0; i < info.edge_count; ++i) {
	//		EdgeId eid = router.GetRouteEdge(info.id, i);
	//		Edge edge = graph.GetEdge(eid);

	//		cout << edge << endl;

	//		if (abs(int(edge.from) - int(edge.to)) == STOPS_COUNT) {
	//			if (!route.empty()) {
	//				string_view stopBegin = v_stops[(route.front().from % STOPS_COUNT)];
	//				string_view stopEnd = v_stops[(route.back().to % STOPS_COUNT)];

	//				const set<string>& busesFrom = stopBuses.at(string(stopBegin));
	//				const set<string>& busesTo = stopBuses.at(string(stopEnd));

	//				set<string> busesRoute;

	//				std::set_intersection(busesFrom.begin(), busesFrom.end(),
	//					busesTo.begin(), busesTo.end(),
	//					std::inserter(busesRoute, busesRoute.begin()));

	//				if (busesRoute.size() == 1) {
	//					double route_weight = 0;
	//					for (auto& edge : route) {
	//						route_weight += edge.weight;
	//					}

	//					ItemRoute ritem{ route_weight, RouteItemType::BUS, route.size(), string(*busesRoute.begin()) };

	//					result.items.emplace_back(ritem);
	//				}
	//				else {
	//					throw invalid_argument("Bus count > 1");
	//				}
	//			}

	//			ItemRoute ritem{ edge.weight, RouteItemType::WAIT, 0, string(v_stops[(edge.from % STOPS_COUNT)]) };
	//			route.clear();

	//			result.items.emplace_back(ritem);
	//		}
	//		else {
	//			route.push_back(move(edge));

	//			if (i == info.edge_count - 1) {
	//				string_view stopBegin = v_stops[(route.front().from % STOPS_COUNT)];
	//				string_view stopEnd = v_stops[(route.back().to % STOPS_COUNT)];

	//				const set<string>& busesFrom = stopBuses.at(string(stopBegin));
	//				const set<string>& busesTo = stopBuses.at(string(stopEnd));

	//				set<string> busesRoute;

	//				std::set_intersection(busesFrom.begin(), busesFrom.end(),
	//					busesTo.begin(), busesTo.end(),
	//					std::inserter(busesRoute, busesRoute.begin()));

	//				if (busesRoute.size() == 1) {
	//					double route_weight = 0;
	//					for (auto& edge : route) {
	//						route_weight += edge.weight;
	//					}

	//					ItemRoute ritem{ route_weight, RouteItemType::BUS, route.size(), string(*busesRoute.begin()) };

	//					result.items.emplace_back(ritem);
	//					route.clear();
	//				}
	//				else {
	//					throw invalid_argument("Bus count > 1");
	//				}
	//			}
	//		}

	//		//result.items.emplace_back(edge.weight, );
	//	}
	//}
	return DirectoryTransport::RouteResult(); // result;
}

int DirectoryTransport::calcStopCount(const vector<string>& stops_) const {
	return stops_.size();
}

int DirectoryTransport::calcUniqueStopCount(const vector<string>& stops_) const {
	set<string_view> uniq(stops_.begin(), stops_.end());
	return uniq.size();
}

double DirectoryTransport::calcRouteLength(const vector<string>& stops_) const {
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

double DirectoryTransport::calcRouteLengthGeo(const vector<string>& stops_) const {
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

double DirectoryTransport::curvatureRoute(double realDist, double geoDist) {
	return realDist / geoDist;
}

double DirectoryTransport::gradToRad(double grad) const {
	const double pi = 3.1415926535;
	return grad * pi / 180.;
}

double DirectoryTransport::distance(double lat1, double lon1, double lat2, double lon2) const {
	const double radiusEarth = 6371000.;
	return acos(sin(lat1) * sin(lat2)
		+ cos(lat1) * cos(lat2) * cos(abs(lon1 - lon2))
	) * radiusEarth;
}

float DirectoryTransport::calcTime(int distance)
{
	return (distance * 0.06f) / busVelocity;
}

void DirectoryTransport::createRouter()
{
	if (!ptrRouter) {
		const size_t STOPS_COUNT = stops.size();
		const size_t VERTEX_COUNT = STOPS_COUNT * 2;
		DirectedWeightedGraph<float> graph(VERTEX_COUNT);

		vector<string_view> v_stops;
		v_stops.reserve(STOPS_COUNT);
		unordered_map<string_view, size_t> m_stops;
		m_stops.reserve(STOPS_COUNT);
		for (const auto&[name, p] : stops) {
			v_stops.push_back(name);
			m_stops[name] = v_stops.size() - 1;
		}

		int count = 0;
		for (auto& str : v_stops) {
			cout << "[" << count++ << "]: " << str << endl;
		}


		for (const auto&[stop, distToStops] : distanceBetweenStops) {
			{
				Edge<float> edge;
				edge.from = m_stops.at(stop);
				edge.to = edge.from + STOPS_COUNT;
				edge.weight = waitTime;
				graph.AddEdge(edge);

				cout << "Add edge: " << edge << endl;
			}
			{
				Edge<float> edge;
				edge.to = m_stops.at(stop);
				edge.from = edge.to + STOPS_COUNT;
				edge.weight = waitTime;
				graph.AddEdge(edge);

				cout << "Add edge: " << edge << endl;
			}
			for (const auto&[name, dist] : distToStops) {
				Edge<float> edge;
				edge.from = m_stops.at(stop);
				edge.to = m_stops.at(name);
				edge.weight = (dist * 60.) / (busVelocity * 1000.);
				graph.AddEdge(edge);

				cout << "Add edge: " << edge << endl;
			}
		}

		ptrRouter = make_unique<Router<float>>(graph);
	}
}

struct VertexData {
	size_t id;
	string stopName;
	string busName;
	vector<string>::iterator busStopIt;
};

void DirectoryTransport::formationVertexes()
{
	size_t id = 0;
	vector<VertexData> vertexes;
	vector<Edge<float>> edges;
	unordered_map<string, size_t> stopToVertexId;

	// добавляем начальные остановки
	for (auto&[stop, crd] : stops) {
		VertexData data;
		data.id = id;
		data.stopName = stop;
		vertexes.push_back(data);
		stopToVertexId[stop] = id;
		++id;
	}

	for (auto&[bus, stops_] : buses) {
		map<string, size_t> cur_stops;
		size_t prev_id;
		for (auto it = stops_.begin(); it != stops_.end(); ++it) {
			size_t cur_id = id;
			if (cur_stops.count(*it) == 0) {
				VertexData data;
				data.id = cur_id;
				data.stopName = *it;
				data.busName = bus;
				data.busStopIt = it;
				vertexes.push_back(data);
				cur_stops[*it] = cur_id;
			}
			else {
				cur_id = cur_stops[*it];
			}

			Edge<float> edge;
			if (stops.count(data.stopName)) {
				size_t curId = stopToVertexId.at(data.stopName);
				edge.from = curId;
				edge.to = id;
				edge.weight = waitTime;
			}
			edges.push_back(edge);

			// начинаем со второй остановки
			if (it != stops_.begin()) {
				auto& stopFrom = *prev(it);
				auto& stopTo = *it;
				int dist = distanceBetweenStops.at(stopFrom).at(stopTo);
				edge.from = id - 1;
				edge.to = id;
				edge.weight = calcTime(dist);
			}
		}
	}
}
