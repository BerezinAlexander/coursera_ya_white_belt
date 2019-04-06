#include "test_runner.h"

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
	{}

	void Read(int user_id, int page_count) {
		
		if (users.empty()) {
			users[user_id] = page_count;
			pages[page_count].insert(user_id);
			return;
		}

		if (users.count(user_id) == 0) {
			users[user_id] = page_count;
			pages[page_count].insert(user_id);
		}
		else {
			int old_page = users[user_id];
			pages[old_page].erase(user_id);
			pages[page_count].insert(user_id);
			users[user_id] = page_count;
		}
	}

	double Cheer(int user_id) const {
		if (users.count(user_id) == 0)
			return 0;

		if (users.size() == 1)
			return 1;

		int count_users = 0;
		int page = users.at(user_id);
		for (auto it = pages.begin(); it != pages.end(); ++it) {
			if (it->first == page)
				break;

			count_users += it->second.size();
		}

		int all_users = users.size() - 1;

		return (count_users * 1.) / all_users;
	}

private:
	// Статическое поле не принадлежит какому-то конкретному
	// объекту класса. По сути это глобальная переменная,
	// в данном случае константная.
	// Будь она публичной, к ней можно было бы обратиться снаружи
	// следующим образом: ReadingManager::MAX_USER_COUNT.
	static const int MAX_USER_COUNT_ = 100'000;

	map<int, int> users; // <Id, Page>
	map<int, set<int>> pages; // Page, set<Id>
};


void Test() {
	ReadingManager manager;

	ASSERT_EQUAL(manager.Cheer(5), 0);
	manager.Read(1, 10);
	ASSERT_EQUAL(manager.Cheer(1), 1);
	manager.Read(2, 5);
	manager.Read(3, 7);
	ASSERT_EQUAL(manager.Cheer(2), 0);
	ASSERT_EQUAL(manager.Cheer(3), 0.5);
	manager.Read(3, 10);
	ASSERT_EQUAL(manager.Cheer(3), 0.5);
	manager.Read(3, 11);
	ASSERT_EQUAL(manager.Cheer(3), 1);
	ASSERT_EQUAL(manager.Cheer(1), 0.5);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, Test);

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