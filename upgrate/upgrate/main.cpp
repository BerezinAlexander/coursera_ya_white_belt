#include "test_runner.h"

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <numeric>
#include <map>
#include <algorithm>
#include <set>

//#include "LogDuration.h"

using namespace std;

//class RouteManager {
//public:
//	void AddRoute(int start, int finish) {
//		reachable_lists_[start].push_back(finish);
//		reachable_lists_[finish].push_back(start);
//	}
//	int FindNearestFinish(int start, int finish) const {
//		int result = abs(start - finish);
//		if (reachable_lists_.count(start) < 1) {
//			return result;
//		}
//		const vector<int>& reachable_stations = reachable_lists_.at(start);
//		if (!reachable_stations.empty()) {
//			result = min(
//				result,
//				abs(finish - *min_element(
//					begin(reachable_stations), end(reachable_stations),
//					[finish](int lhs, int rhs) { return abs(lhs - finish) < abs(rhs - finish); }
//				))
//			);
//		}
//		return result;
//	}
//private:
//	map<int, vector<int>> reachable_lists_;
//};

class RouteManager {
public:
	void AddRoute(int start, int finish) {
		reachable_lists_[start].insert(finish);
		reachable_lists_[finish].insert(start);
	}
	int FindNearestFinish(int start, int finish) const {
		int result = abs(start - finish);
		if (reachable_lists_.find(start) != reachable_lists_.end()) {
			const set<int>& reachable_stations = reachable_lists_.at(start);
			if (!reachable_stations.empty()) {
				int res = 0;
				auto it = reachable_stations.lower_bound(finish);
				if (it == reachable_stations.end()) {
					res = abs(*(--it) - finish);
				}
				else if (it == reachable_stations.begin()) {
					res = abs( *(it)  - finish);
				}
				else {
					res = min(abs(*prev(it) - finish), abs(*it - finish));
				}
				result = min(result, res);
			}
		}
		return result;
	}
private:
	map<int, set<int>> reachable_lists_;
};

//void Test() {
//	RouteManager routes;
//
//	routes.AddRoute(-2, 5);
//	routes.AddRoute(10, 4);
//	routes.AddRoute(5, 8);
//
//	ASSERT_EQUAL(routes.FindNearestFinish(4, 10), 0);
//	ASSERT_EQUAL(routes.FindNearestFinish(4, -2), 6);
//	ASSERT_EQUAL(routes.FindNearestFinish(5, 0), 2);
//	ASSERT_EQUAL(routes.FindNearestFinish(5, 100), 92);	
//}

int main() {	
	//TestRunner tr;
	//RUN_TEST(tr, Test);

	//RouteManager routes;

	//{
	//	LOG_DURATION("All query: ");
	//	int query_count = 100000;
	//	for (int i = 0; i < query_count / 2; ++i) {
	//		routes.AddRoute(-i * 3 - 4, i * 3 + 5);
	//	}

	//	for (int i = 0; i < query_count / 2; ++i) {
	//		routes.FindNearestFinish(-i * 5 - 3, i * 3 + 7);
	//		//cout << routes.FindNearestFinish(-i * 5 - 3, i * 3 + 7) << "\n";
	//	}
	//}

	RouteManager routes;

	int query_count;
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) {
		string query_type;
		cin >> query_type;
		int start, finish;
		cin >> start >> finish;
		if (query_type == "ADD") {
			routes.AddRoute(start, finish);
		}
		else if (query_type == "GO") {
			cout << routes.FindNearestFinish(start, finish) << "\n";
		}
	}


#ifdef _MSC_VER
	system("pause");
#endif
}