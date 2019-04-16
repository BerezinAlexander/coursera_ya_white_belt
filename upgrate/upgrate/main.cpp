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
	using Id = int;
	using Prior = int;

	PriorityCollection() {
		const int max_count = 1'000'000;
		data.resize(max_count);
		valid.resize(max_count, false);
	}

	//struct Id {
	//	typename list<T>::iterator it;
	//	int prior;
	//	int position;

	//	Id() : prior(0), position(0) {}

	//	Id(typename list<T>::iterator it_, int prior_, int pos_) : it(it_), prior(prior_), position(pos_) {}

	//	Id(const Id& other) : it(other.it), prior(other.prior), position(other.position) {}

	//	bool operator==(const Id& rhs) const { return (*it == *rhs.it); }
	//	bool operator!=(const Id& rhs) const { return (*it != *rhs.it); }
	//	bool operator> (const Id& rhs) const { 
	//		if (prior == rhs.prior)
	//			return position > rhs.position;
	//		else
	//			return prior > rhs.prior;
	//	}
	//	bool operator>=(const Id& rhs) const { 
	//		if (prior == rhs.prior)
	//			return position >= rhs.position;
	//		else
	//			return prior > rhs.prior;
	//	}
	//	bool operator< (const Id& rhs) const { 
	//		if (prior == rhs.prior)
	//			return position < rhs.position;
	//		else
	//			return prior < rhs.prior;
	//	}
	//	bool operator<=(const Id& rhs) const { 
	//		if (prior == rhs.prior)
	//			return position <= rhs.position;
	//		else
	//			return prior < rhs.prior;
	//	}
	//};


	// �������� ������ � ������� �����������
	// � ������� ����������� � ������� ��� �������������
	Id Add(T object) {
		data[count] = { move(object), 0 };
		valid[count] = true;
		priority.insert({0, count});
		return count++;
	}

	// �������� ��� �������� ��������� [range_begin, range_end)
	// � ������� �����������, ������� �������� �� ��������������
	// � �������� [ids_begin, ...)
	template <typename ObjInputIt, typename IdOutputIt>
	void Add(ObjInputIt range_begin, ObjInputIt range_end,
		IdOutputIt ids_begin)
	{
		for (auto it = range_begin; it != range_end; ++it) {
			*ids_begin++ = move(Add(move(*it)));
		}
	}

	// ����������, ����������� �� ������������� ������-����
	// ����������� � ���������� �������
	bool IsValid(Id id) const {
		return valid[id];
	}

	// �������� ������ �� ��������������
	const T& Get(Id id) const {
		return data[id].first;
	}

	// ��������� ��������� ������� �� 1
	void Promote(Id id) {
		priority.erase({ data[id].second , id });
		priority.insert({ ++data[id].second , id });
	}

	// �������� ������ � ������������ ����������� � ��� ���������
	pair<const T&, int> GetMax() const {
		auto& p = *prev(priority.cend());
		return { data[p.second].first, p.first };
	}

	// ���������� GetMax, �� ������� ������� �� ����������
	pair<T, int> PopMax() {
		auto& p = *prev(priority.end());
		Id id = p.second;
		valid[id] = false;
		priority.erase(prev(priority.end()));
		return move(data[id]);
	}

private:
	// ��������� ���� � ������
	set<pair<Prior, Id>> priority; // prior, index
	//list<T> data;

	vector<pair<T, Prior>> data;
	vector<bool> valid;

	int count = 0;
};


class StringNonCopyable : public string {
public:
	using string::string;  // ��������� ������������ ������������ ������
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