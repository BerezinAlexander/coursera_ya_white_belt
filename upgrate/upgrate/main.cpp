//#include "test_runner.h"

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <numeric>
#include <map>
#include <algorithm>
#include <set>
#include <iomanip>
#include <utility>

//#include "LogDuration.h"

using namespace std;

class ReadingManager {
public:
	ReadingManager()
	{		
		pages_u.resize(1001);
		for (int i = 0; i < 1001; ++i)
			pages_u[i] = 0;
	}

	void Read(int user_id, int page_count) {
		if (users.count(user_id) != 0) {
			--pages_u[users[user_id]];
		}

		users[user_id] = page_count;
		++pages_u[page_count];
	}

	double Cheer(int user_id) const {
		if (users.count(user_id) == 0)
			return 0;

		if (users.size() == 1)
			return 1;

		int count_users = 0;
		int page = users.at(user_id);
		for (int i = 0; i < page; ++i) {
			count_users += pages_u[i];
		}

		return (count_users * 1.) / (users.size() - 1);
	}

private:
	static const int MAX_USER_COUNT_ = 100'000;

	map<int, int> users; // <Id, Page>
	vector<int> pages_u;
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

	ReadingManager manager;

	int query_count = 43;
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) {
		string query_type;
		cin >> query_type;
		int user_id;
		cin >> user_id;

		if (query_type == "READ") {
			int page_count;
			cin >> page_count;
			manager.Read(user_id, page_count);
		}
		else if (query_type == "CHEER") {
			cout << setprecision(6) << manager.Cheer(user_id) << "\n";
		}
	}


#ifdef _MSC_VER
	system("pause");
#endif
}