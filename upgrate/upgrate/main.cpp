#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
#include <list>
#include <map>
#include <iterator>

using namespace std;

template <class T>
class ObjectPool {
public:
	T* Allocate() {
		T* obj = nullptr;
		if (!freeObjects.empty()) {
			obj = freeObjects.front();
			freeObjects.pop();			
		}
		else {
			obj = new T();
		}
		alocObjects.insert(obj);
		//sAlocObjects.insert(obj);
		return obj;
	}

	T* TryAllocate() {
		T* obj = nullptr;
		if (!freeObjects.empty()) {
			obj = freeObjects.front();
			alocObjects.insert(obj);
			freeObjects.pop();
			//sAlocObjects.insert(obj);
		}
		return obj;
	}

	void Deallocate(T* object) {
		if (alocObjects.count(object)) {
			freeObjects.push(object);
			alocObjects.erase(object);
		}
		else {
			throw invalid_argument("");
		}
	}

	~ObjectPool() {
		while (!freeObjects.empty()) {
			delete freeObjects.front();
			freeObjects.pop();
		}
		for (auto obj : alocObjects) {
			delete obj;
			obj = nullptr;
		}
		alocObjects.clear();
	}

private:
	queue<T*> freeObjects;
	set<T*> alocObjects;
};

void TestObjectPool() {
	ObjectPool<string> pool;

	auto p1 = pool.Allocate();
	auto p2 = pool.Allocate();
	auto p3 = pool.Allocate();

	*p1 = "first";
	*p2 = "second";
	*p3 = "third";

	pool.Deallocate(p2);
	ASSERT_EQUAL(*pool.Allocate(), "second");

	pool.Deallocate(p3);
	pool.Deallocate(p1);
	ASSERT_EQUAL(*pool.Allocate(), "third");
	ASSERT_EQUAL(*pool.Allocate(), "first");

	pool.Deallocate(p1);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestObjectPool);

#ifdef _MSC_VER
	system("pause");
#endif
}