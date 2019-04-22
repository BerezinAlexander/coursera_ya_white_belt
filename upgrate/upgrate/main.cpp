#include "test_runner.h"
#include "profile.h"

#include <cstdint>
#include <iostream>

#include <numeric>

#include <algorithm>
#include <random>
#include <memory>
#include <iterator>

#include <array>
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <tuple>
#include <string_view>

#include <future>
#include <mutex>
#include <thread>


using namespace std;

template <typename K, typename V>
class ConcurrentMap {
public:
	static_assert(is_integral_v<K>, "ConcurrentMap supports only integer keys");

	struct Access {
		V& ref_to_value;
		lock_guard<mutex> guard;
	};

	explicit ConcurrentMap(size_t bucket_count)
		: maps(bucket_count)
		, mutexes(bucket_count)
	{
	}

	Access operator[](const K& key) {
		for (int i = 0; i < maps.size(); ++i) {
			if (maps[i].count(key)) {
				return {maps[i][key], lock_guard<mutex>(mutexes[i]) };
			}
		}

		size_t smoller = findSmollerMap();
		mutexes[smoller].lock();
		maps[smoller][key] = V();
		mutexes[smoller].unlock();

		return { maps[smoller][key], lock_guard(mutexes[smoller])};
	}

	map<K, V> BuildOrdinaryMap() {
		map<K, V> result;
		for (int i = 0; i < maps.size(); ++i) {
			lock_guard<mutex> g(mutexes[i]);
			for (auto&[key, value] : maps[i]) {
				result[key] += value;
			}
		}
		return result;
	}

private:
	vector<map<K, V>> maps;
	vector<mutex> mutexes;

	size_t findSmollerMap() {
		if (maps.empty())
			return -1;

		size_t smoller = 0;
		size_t smollerSize = 0;

		//{
		//	lock_guard<mutex> g(mutexes[0]);
			smollerSize = maps[0].size();
		//}

		for (size_t i = 1; i < maps.size(); ++i) {
			//lock_guard<mutex> g(mutexes[i]);
			if (smollerSize < maps[i].size()) {
				smoller = i;
				smollerSize = maps[i].size();
			}
		}

		return smoller;
	}
};

void RunConcurrentUpdates(
	ConcurrentMap<int, int>& cm, size_t thread_count, int key_count
) {
	auto kernel = [&cm, key_count](int seed) {
		vector<int> updates(key_count);
		iota(begin(updates), end(updates), -key_count / 2);
		shuffle(begin(updates), end(updates), default_random_engine(seed));

		for (int i = 0; i < 2; ++i) {
			for (auto key : updates) {
				cm[key].ref_to_value++;
			}
		}
	};

	vector<future<void>> futures;
	for (size_t i = 0; i < thread_count; ++i) {
		futures.push_back(async(kernel, i));
	}
}

void TestConcurrentUpdate() {
	const size_t thread_count = 3;
	const size_t key_count = 50000;

	ConcurrentMap<int, int> cm(thread_count);
	RunConcurrentUpdates(cm, thread_count, key_count);

	const auto result = cm.BuildOrdinaryMap();
	ASSERT_EQUAL(result.size(), key_count);
	for (auto&[k, v] : result) {
		AssertEqual(v, 6, "Key = " + to_string(k));
	}
}

void TestReadAndWrite() {
	ConcurrentMap<size_t, string> cm(5);

	auto updater = [&cm] {
		for (size_t i = 0; i < 50000; ++i) {
			cm[i].ref_to_value += 'a';
		}
	};
	auto reader = [&cm] {
		vector<string> result(50000);
		for (size_t i = 0; i < result.size(); ++i) {
			result[i] = cm[i].ref_to_value;
		}
		return result;
	};

	auto u1 = async(updater);
	auto r1 = async(reader);
	auto u2 = async(updater);
	auto r2 = async(reader);

	u1.get();
	u2.get();

	for (auto f : { &r1, &r2 }) {
		auto result = f->get();
		ASSERT(all_of(result.begin(), result.end(), [](const string& s) {
			return s.empty() || s == "a" || s == "aa";
			}));
	}
}

void TestSpeedup() {
	{
		ConcurrentMap<int, int> single_lock(1);

		LOG_DURATION("Single lock");
		RunConcurrentUpdates(single_lock, 4, 50000);
	}
	{
		ConcurrentMap<int, int> many_locks(100);

		LOG_DURATION("100 locks");
		RunConcurrentUpdates(many_locks, 4, 50000);
	}
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestConcurrentUpdate);
	RUN_TEST(tr, TestReadAndWrite);
	RUN_TEST(tr, TestSpeedup);

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}