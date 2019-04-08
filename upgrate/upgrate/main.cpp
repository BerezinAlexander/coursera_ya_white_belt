#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include <set>
#include <deque>
#include <algorithm>

using namespace std;

static double current_time = 0;

struct Record {
	double time;
	int client_id;
	int room_count;

	Record(const double& time_, const int& client_id_,
		const int& room_count_)
		: time(time_), client_id(client_id_), room_count(room_count_)
	{}
};

class HotelManager {
public:
	HotelManager() = default;
	~HotelManager() = default;

	void reservation(const double& time, const int& client_id, 
		const int& room_count)
	{
		records.emplace_front(time, client_id, room_count);
		rooms_count += room_count;
		cl_rooms[client_id] += room_count;

		for (auto it = prev(records.end()); it >= records.begin(); --it) {
			if (current_time - 86400 < it->time) {
				records.erase(++it, records.end());
				break;
			}
			else {
				cl_rooms[it->client_id] -= it->room_count;
				if (cl_rooms[it->client_id] == 0)
					cl_rooms.erase(it->client_id);
				rooms_count -= it->room_count;
			}

		}

		//find_if(records.rbegin(), records.rend())

		//for (auto rit = records.rbegin(); it != records.rend(); ++rit) {
		//	if (current_time - 86400 >= rit->time) {
		//		clients.insert(rit->client_id);
		//	}
		//}
	}

	size_t clients() {
		//set<int> clients;
		//for (auto rit = records.rbegin(); rit != records.rend(); ++rit) {
		//	if (current_time - 86400 < rit->time) {
		//		clients.insert(rit->client_id);
		//	}
		//}
		//return clients.size();
		return cl_rooms.size();
	}

	int rooms() {
		//int rooms_count = 0;
		//for (auto rit = records.rbegin(); rit != records.rend(); ++rit) {
		//	if (current_time - 86400 < rit->time) {
		//		rooms_count += rit->room_count;
		//	}
		//}
		//return rooms_count;

		return rooms_count;
	}

private:
	deque<Record> records;
	map<int, int> cl_rooms;
	int rooms_count;
};

class ReservationManager {
public:
	ReservationManager() = default;
	~ReservationManager() = default;

	void book(const double& time, const string& hotel_name,
		const int& client_id, const int& room_count)
	{
		if (hotels.count(hotel_name) == 0) {
			hotels[hotel_name] = HotelManager();
		}
		
		hotels[hotel_name].reservation(time, client_id, room_count);
	}

	size_t clients(const string& hotel_name) {
		if (hotels.count(hotel_name) == 0) {
			return 0;
		}

		return hotels[hotel_name].clients();
	}

	int rooms(const string& hotel_name)	{
		if (hotels.count(hotel_name) == 0) {
			return 0;
		}

		return hotels[hotel_name].rooms();
	}

private:
	map<string, HotelManager> hotels;
};


//void Test() {
//	ReadingManager manager;
//
//	ASSERT_EQUAL(manager.Cheer(5), 0);
//	manager.Read(1, 10);
//	ASSERT_EQUAL(manager.Cheer(1), 1);
//	manager.Read(2, 5);
//	manager.Read(3, 7);
//	ASSERT_EQUAL(manager.Cheer(2), 0);
//	ASSERT_EQUAL(manager.Cheer(3), 0.5);
//	manager.Read(3, 10);
//	ASSERT_EQUAL(manager.Cheer(3), 0.5);
//	manager.Read(3, 11);
//	ASSERT_EQUAL(manager.Cheer(3), 1);
//	ASSERT_EQUAL(manager.Cheer(1), 0.5);
//}

int main() {
	//TestRunner tr;
	//RUN_TEST(tr, Test);

	// Для ускорения чтения данных отключается синхронизация
	// cin и cout с stdio,
	// а также выполняется отвязка cin от cout
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ReservationManager manager;

	int query_count;
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) {
		string query_type;
		cin >> query_type;

		if (query_type == "BOOK") {
			double time;
			string hotel_name;
			int client_id;
			int room_count;
			cin >> time >> hotel_name >> client_id >> room_count;

			current_time = time;

			manager.book(time, hotel_name, client_id, room_count);
		}
		else if (query_type == "CLIENTS") {
			string hotel_name;
			cin >> hotel_name;

			cout << manager.clients(hotel_name) << "\n";
		}
		else if (query_type == "ROOMS") {
			string hotel_name;
			cin >> hotel_name;

			cout << manager.rooms(hotel_name) << "\n";
		}
	}


#ifdef _MSC_VER
	system("pause");
#endif
}