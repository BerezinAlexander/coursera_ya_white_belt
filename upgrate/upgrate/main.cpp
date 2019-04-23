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
		: segments(bucket_count)
	{
	}

	Access operator[](const K& key) {
		size_t indexFind = 0;
		bool isFind = false;
		for (int i = 0; i < segments.size(); ++i) {
			auto& col = getSegment(i).ref_to_value;
			if (col.count(key)) {
				indexFind = i;
				
				return { segments[indexFind].col[key], lock_guard(segments[indexFind].m) };

				isFind = true;
				break;
			}
		}

		if (!isFind) {
			size_t smollerSize = numeric_limits<size_t>::infinity();
			for (size_t i = 0; i < segments.size(); ++i) {
				auto& col = getSegment(i).ref_to_value;
				size_t curSize = col.size();
				if (curSize < smollerSize) {
					indexFind = i;
					smollerSize = curSize;
				}
			}

			return { segments[indexFind].col[key], lock_guard(segments[indexFind].m) };
		}

		//return { segments[indexFind].col[key], lock_guard(segments[indexFind].m) };
	}

	map<K, V> BuildOrdinaryMap() {
		map<K, V> result;
		for (int i = 0; i < segments.size(); ++i) {
			auto& col = getSegment(i).ref_to_value;
			for (auto&[key, value] : col) {
				result[key] += value;
			}
		}
		return result;
	}

private:
	//vector<map<K, V>> maps;
	//vector<mutex> mutexes;

	struct Segment {
		map<K, V> col;
		mutex m;
	};
	
	vector<Segment> segments;

	struct RefSegment {
		map<K, V>& ref_to_value;
		lock_guard<mutex> guard;
	};

	RefSegment getSegment(size_t index) {
		return { segments[index].col, lock_guard<mutex>(segments[index].m) };
	}

	//Access getOrAddValue(const K& key) {
	//	for (auto& seg : segments) {
	//		lock_guard<mutex> g(seg.m);
	//		if (col.find(key) != col.end())
	//			return { lock_guard<mutex>(seg.m), col[key] };
	//	}

	//	if(segments.empty())


	//	size_t max_size = numeric_limits<size_t>::infinity;
	//	auto& min_seg = segments[0];
	//	for (auto& seg : segments) {
	//		lock_guard<mutex> g(seg.m);
	//		sizes.insert(seg.col.size());
	//	}


	//}

	size_t findSmollerMap() {
		size_t smoller = -1;
		size_t smollerSize = -1;
		
		for (size_t i = 0; i < segments.size(); ++i) {
			auto& col = getSegment(i).ref_to_value;
			size_t curSize = col.size();
			if (curSize < smollerSize) {
				smoller = i;
				smollerSize = curSize;
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