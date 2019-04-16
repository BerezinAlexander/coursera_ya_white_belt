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


using namespace std;

template <typename T>
class PriorityCollection {
public:
	//using Id = typename list<T>::iterator;

	struct Id {
		typename list<T>::iterator it;
		int prior;
		int position;

		Id() : prior(0), position(0) {}

		Id(typename list<T>::iterator it_, int prior_, int pos_) : it(it_), prior(prior_), position(pos_) {}

		Id(const Id& other) : it(other.it), prior(other.prior), position(other.position) {}

		bool operator==(const Id& rhs) const { return (*it == *rhs.it); }
		bool operator!=(const Id& rhs) const { return (*it != *rhs.it); }
		bool operator> (const Id& rhs) const { 
			if (prior == rhs.prior)
				return position > rhs.position;
			else
				return prior > rhs.prior;
		}
		bool operator>=(const Id& rhs) const { 
			if (prior == rhs.prior)
				return position >= rhs.position;
			else
				return prior > rhs.prior;
		}
		bool operator< (const Id& rhs) const { 
			if (prior == rhs.prior)
				return position < rhs.position;
			else
				return prior < rhs.prior;
		}
		bool operator<=(const Id& rhs) const { 
			if (prior == rhs.prior)
				return position <= rhs.position;
			else
				return prior < rhs.prior;
		}
	};


	// Добавить объект с нулевым приоритетом
	// с помощью перемещения и вернуть его идентификатор
	Id Add(T object) {
		data.push_back(move(object));
		Id id(prev(data.end()), 0, count++);
		priority.insert(id);
		return id;
	}

	// Добавить все элементы диапазона [range_begin, range_end)
	// с помощью перемещения, записав выданные им идентификаторы
	// в диапазон [ids_begin, ...)
	template <typename ObjInputIt, typename IdOutputIt>
	void Add(ObjInputIt range_begin, ObjInputIt range_end,
		IdOutputIt ids_begin)
	{
		for (auto it = range_begin; it != range_end; ++it) {
			*ids_begin++ = move(Add(move(*it)));
		}
	}

	// Определить, принадлежит ли идентификатор какому-либо
	// хранящемуся в контейнере объекту
	bool IsValid(Id id) const {
		return priority.count(id);
	}

	// Получить объект по идентификатору
	const T& Get(Id id) const {
		return *id.it;
	}

	// Увеличить приоритет объекта на 1
	void Promote(Id id) {
		auto it = find_if(priority.begin(), priority.end(), [&id](const Id& cur) {return cur == id; });
		if (it != priority.end()) {
			Id cur = *it;
			cur.prior++;
			priority.erase(it);
			priority.insert(cur);
		}
	}

	// Получить объект с максимальным приоритетом и его приоритет
	pair<const T&, int> GetMax() const {
		const Id& id = *prev(priority.cend());
		return { *(id.it), id.prior };
	}

	// Аналогично GetMax, но удаляет элемент из контейнера
	pair<T, int> PopMax() {
		auto it = prev(priority.end());
		pair<T, int> p = make_pair(move(*(it->it)), it->prior);
		data.erase(it->it);
		priority.erase(it);
		return p;
	}

private:
	// Приватные поля и методы
	set<Id> priority;
	list<T> data;

	int count = 0;
};


class StringNonCopyable : public string {
public:
	using string::string;  // Позволяет использовать конструкторы строки
	StringNonCopyable(const StringNonCopyable&) = delete;
	StringNonCopyable(StringNonCopyable&&) = default;
	StringNonCopyable& operator=(const StringNonCopyable&) = delete;
	StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
	PriorityCollection<StringNonCopyable> strings;
	const auto white_id = strings.Add("white");
	const auto yellow_id = strings.Add("yellow");
	const auto red_id = strings.Add("red");

	strings.Promote(yellow_id);
	for (int i = 0; i < 2; ++i) {
		strings.Promote(red_id);
	}
	strings.Promote(yellow_id);
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "red");
		ASSERT_EQUAL(item.second, 2);
	}
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "yellow");
		ASSERT_EQUAL(item.second, 2);
	}
	{
		const auto item = strings.PopMax();
		ASSERT_EQUAL(item.first, "white");
		ASSERT_EQUAL(item.second, 0);
	}
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestNoCopy);

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}