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

		/*: user_page_counts_(MAX_USER_COUNT_ + 1, 0),
		sorted_users_(),
		user_positions_(MAX_USER_COUNT_ + 1, -1) {}*/

	void Read(int user_id, int page_count) {
		if (users.empty()) {
			users[user_id] = page_count;
			pages[page_count] = set<int>();
			return;
		}
		
		if (users.count(user_id) == 0) {
			users[user_id] = page_count;

			auto it = pages.lower_bound(page_count);
			if (it != pages.end()) {
				if (it->first > page_count) { 
					pages[page_count] = set<int>();
					for_each(it, pages.end(),
						[&user_id](auto& pair) {
							pair.second.insert(user_id);
						}
					);

				}
				else if (it->first == page_count) {
					if (++it != pages.end()) {
						for_each(it, pages.end(),
								[&user_id](auto& pair) {
									pair.second.insert(user_id);
								}
						);
					}
				}
			}
			else {
				auto last_pair_it = --pages.end();
				pages[page_count] = last_pair_it->second;
				pages[page_count].insert(last_pair_it->first);
			}
		}
		else {
			int page = users.at(user_id);
			auto findIt = pages.find(page);
			auto findLowIt = pages.lower_bound(page_count);
			if (findLowIt != pages.end()) {
				for_each(++findIt, findLowIt,
					[&user_id](auto& pair) {
					pair.second.erase(user_id);
				});

				if (findLowIt->first > page_count) {
					auto last_pair_it = --findLowIt;
					pages[page_count] = last_pair_it->second;
					pages[page_count].insert(last_pair_it->first);
				}
			}
			users[user_id] = page_count;
			pages[page_count] = findIt->second;
			pages.erase(findIt);
		}


		//if (user_page_counts_[user_id] == 0) {
		//	AddUser(user_id);
		//}
		/*user_page_counts_[user_id] = page_count;
		int& position = user_positions_[user_id];
		while (position > 0 && page_count > user_page_counts_[sorted_users_[position - 1]]) {
			SwapUsers(position, position - 1);
		}*/
	}

	double Cheer(int user_id) const {
		if (users.count(user_id) == 0)
			return 0;

		if (users.at(user_id) == 0)
			return 0;

		if (users.size() == 1)
			return 1;

		const set<int>& ratings = pages.at(users.at(user_id));
		int count = 0;
		for (auto it = ratings.begin(); it != ratings.end(); it++) {
			++count;
		}

		return count * 1.0 / ratings.size();

/*



		if (user_page_counts_[user_id] == 0) {
			return 0;
		}
		const int user_count = GetUserCount();
		if (user_count == 1) {
			return 1;
		}
		const int page_count = user_page_counts_[user_id];
		int position = user_positions_[user_id];
		while (position < user_count &&
			user_page_counts_[sorted_users_[position]] == page_count) {
			++position;
		}
		if (position == user_count) {
			return 0;
		}*/
		// ѕо умолчанию деление целочисленное, поэтому
		// нужно привести числитель к типу double.
		// ѕростой способ сделать это Ч умножить его на 1.0.
		//return (user_count - position) * 1.0 / (user_count - 1);
	}

private:
	// —татическое поле не принадлежит какому-то конкретному
	// объекту класса. ѕо сути это глобальна€ переменна€,
	// в данном случае константна€.
	// Ѕудь она публичной, к ней можно было бы обратитьс€ снаружи
	// следующим образом: ReadingManager::MAX_USER_COUNT.
	static const int MAX_USER_COUNT_ = 100'000;

	map<int, int> users; // <Id, Page>
	map<int, set<int>> pages; // Page, set<Id>

	//vector<int> user_page_counts_;
	//vector<int> sorted_users_;   // отсортированы по убыванию количества страниц
	//vector<int> user_positions_; // позиции в векторе sorted_users_

	//int GetUserCount() const {
	//	return users.size();
	//	//return sorted_users_.size();
	//}
	//void AddUser(int user_id) {
	//	users[user_id];

	//	//sorted_users_.push_back(user_id);
	//	//user_positions_[user_id] = sorted_users_.size() - 1;
	//}
	//void SwapUsers(int lhs_position, int rhs_position) {
	//	const int lhs_id = sorted_users_[lhs_position];
	//	const int rhs_id = sorted_users_[rhs_position];
	//	swap(sorted_users_[lhs_position], sorted_users_[rhs_position]);
	//	swap(user_positions_[lhs_id], user_positions_[rhs_id]);
	//}
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

	// ƒл€ ускорени€ чтени€ данных отключаетс€ синхронизаци€
	// cin и cout с stdio,
	// а также выполн€етс€ отв€зка cin от cout
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ReadingManager manager;

	int query_count;
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