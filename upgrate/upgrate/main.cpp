#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

struct Record {
	string id;
	string title;
	string user;
	int timestamp;
	int karma;
};

// Реализуйте этот класс
class Database {
public:
	bool Put(const Record& record) {
		if (!GetById(record.id)) {
			records[record.id] = record;
			Iters iters;
			iters.iter_timestamp = timestamp_records.insert(pair(record.timestamp, &records[record.id]));
			iters.iter_karma = karma_records.insert(pair(record.karma, &records[record.id]));
			iters.iter_user = user_records.insert(pair(record.user, &records[record.id]));
			id_iters[record.id] = move(iters);
			return true;
		}
		return false;
	}

	const Record* GetById(const string& id) const
	{
		return (!records.count(id)) ? nullptr : &records.at(id);
	}
	
	bool Erase(const string& id)
	{
		if (GetById(id)) {
			Iters& iters = id_iters[id];
			timestamp_records.erase(iters.iter_timestamp);
			karma_records.erase(iters.iter_karma);
			user_records.erase(iters.iter_user);
			id_iters.erase(id);
			records.erase(id);
			return true;
		}
		return false;
	}

	template <typename Callback>
	void RangeByTimestamp(int low, int high, Callback callback) const {
		auto itBegin = timestamp_records.lower_bound(low);
		auto itEnd = timestamp_records.upper_bound(high);
		for (auto it = itBegin; it != itEnd; ++it) {
			if (!callback(*(it->second)))
				return;
		}
	}

	template <typename Callback>
	void RangeByKarma(int low, int high, Callback callback) const {
		auto itBegin = karma_records.lower_bound(low);
		auto itEnd = karma_records.upper_bound(high);
		for (auto it = itBegin; it != itEnd; ++it) {
			if (!callback(*(it->second)))
				return;
		}
	}

	template <typename Callback>
	void AllByUser(const string& user, Callback callback) const {
		auto itBegin = user_records.lower_bound(user);
		auto itEnd = user_records.upper_bound(user);
		for (auto it = itBegin; it != itEnd; ++it) {
			if (!callback(*(it->second)))
				return;
		}
	}

private:
	using Iter1 = multimap<int, Record*>::iterator;
	using Iter2 = multimap<string, Record*>::iterator;

	map<string, Record> records;
	multimap<int, Record*> timestamp_records;
	multimap<int, Record*> karma_records;
	multimap<string, Record*> user_records;

	struct Iters {
		Iter1 iter_timestamp;
		Iter1 iter_karma;
		Iter2 iter_user;
	};

	map<string, Iters> id_iters;
};

void TestRangeBoundaries() {
	const int good_karma = 1000;
	const int bad_karma = -10;

	Database db;
	db.Put({ "id1", "Hello there", "master", 1536107260, good_karma });
	db.Put({ "id2", "O>>-<", "general2", 1536107260, bad_karma });

	int count = 0;
	db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
		++count;
		return true;
		});

	ASSERT_EQUAL(2, count);
}

void TestSameUser() {
	Database db;
	db.Put({ "id1", "Don't sell", "master", 1536107260, 1000 });
	db.Put({ "id2", "Rethink life", "master", 1536107260, 2000 });

	int count = 0;
	db.AllByUser("master", [&count](const Record&) {
		++count;
		return true;
		});

	ASSERT_EQUAL(2, count);
}

void TestReplacement() {
	const string final_body = "Feeling sad";

	Database db;
	db.Put({ "id", "Have a hand", "not-master", 1536107260, 10 });
	db.Erase("id");
	db.Put({ "id", final_body, "not-master", 1536107260, -10 });

	auto record = db.GetById("id");
	ASSERT(record != nullptr);
	ASSERT_EQUAL(final_body, record->title);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestRangeBoundaries);
	RUN_TEST(tr, TestSameUser);
	RUN_TEST(tr, TestReplacement);
	return 0;
}
