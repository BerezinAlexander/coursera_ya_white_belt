#include "test_runner.h"
#include "profile.h"

#include <cstdint>
#include <iterator>
#include <numeric>

#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>
#include <list>
#include <deque>

#include <string_view>
#include <tuple>
#include <memory>
#include <iterator>
#include <forward_list>

using namespace std;

template <typename Type, typename Hasher>
class HashSet {
public:
	using BucketList = forward_list<Type>;

public:
	explicit HashSet( size_t num_buckets, const Hasher& hasher_ = {}	) 
		: bucket_count(num_buckets)
		, hasher(hasher_)
		, collection(num_buckets)
	{}

	void Add(const Type& value) {
		size_t index = myHasher(value);
		BucketList& bl = collection.at(index);
		if (find(begin(bl), end(bl), value) == bl.end()) {
			bl.push_front(value);
		}
	}
	bool Has(const Type& value) const {
		size_t index = myHasher(value);
		const BucketList& bl = collection[index];
		if (find(begin(bl), end(bl), value) == bl.end()) {
			return false;
		}
		return true;
	}

	void Erase(const Type& value) {
		size_t index = myHasher(value);
		BucketList& bl = collection.at(index);
		if (find(begin(bl), end(bl), value) != bl.end()) {
			bl.remove(value);
		}
	}
	const BucketList& GetBucket(const Type& value) const {
		size_t index = myHasher(value);
		return collection.at(index);
	}

private:
	size_t bucket_count;
	Hasher hasher;
	vector<BucketList> collection;
	
	size_t myHasher(const Type& value) const {
		return hasher(value) % bucket_count;
	}
};

struct IntHasher {
	size_t operator()(int value) const {
		// Это реальная хеш-функция из libc++, libstdc++.
		// Чтобы она работала хорошо, std::unordered_map
		// использует простые числа для числа бакетов
		return value;
	}
};

struct TestValue {
	int value;

	bool operator==(TestValue other) const {
		return value / 2 == other.value / 2;
	}
};

struct TestValueHasher {
	size_t operator()(TestValue value) const {
		return value.value / 2;
	}
};

void TestSmoke() {
	HashSet<int, IntHasher> hash_set(2);
	hash_set.Add(3);
	hash_set.Add(4);

	ASSERT(hash_set.Has(3));
	ASSERT(hash_set.Has(4));
	ASSERT(!hash_set.Has(5));

	hash_set.Erase(3);

	ASSERT(!hash_set.Has(3));
	ASSERT(hash_set.Has(4));
	ASSERT(!hash_set.Has(5));

	hash_set.Add(3);
	hash_set.Add(5);

	ASSERT(hash_set.Has(3));
	ASSERT(hash_set.Has(4));
	ASSERT(hash_set.Has(5));
}

void TestEmpty() {
	HashSet<int, IntHasher> hash_set(10);
	for (int value = 0; value < 10000; ++value) {
		ASSERT(!hash_set.Has(value));
	}
}

void TestIdempotency() {
	HashSet<int, IntHasher> hash_set(10);
	hash_set.Add(5);
	ASSERT(hash_set.Has(5));
	hash_set.Add(5);
	ASSERT(hash_set.Has(5));
	hash_set.Erase(5);
	ASSERT(!hash_set.Has(5));
	hash_set.Erase(5);
	ASSERT(!hash_set.Has(5));
}

void TestEquivalence() {
	HashSet<TestValue, TestValueHasher> hash_set(10);
	hash_set.Add(TestValue{ 2 });
	hash_set.Add(TestValue{ 3 });

	ASSERT(hash_set.Has(TestValue{ 2 }));
	ASSERT(hash_set.Has(TestValue{ 3 }));

	const auto& bucket = hash_set.GetBucket(TestValue{ 2 });
	const auto& three_bucket = hash_set.GetBucket(TestValue{ 3 });
	ASSERT_EQUAL(&bucket, &three_bucket);

	ASSERT_EQUAL(1, distance(begin(bucket), end(bucket)));
	ASSERT_EQUAL(2, bucket.front().value);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestSmoke);
	RUN_TEST(tr, TestEmpty);
	RUN_TEST(tr, TestIdempotency);
	RUN_TEST(tr, TestEquivalence);

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}