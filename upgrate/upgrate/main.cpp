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

using namespace std;

// Объявляем Group<String> для произвольного типа String
// синонимом vector<String>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор групп — vector<Group<String>>.
template <typename String>
using Group = vector<String>;

// Ещё один шаблонный синоним типа
// позволяет вместо громоздкого typename String::value_type
// использовать Char<String>
template <typename String>
using Char = typename String::value_type;


template <typename String>
using Key = String;


template <typename String>
Key<String> ComputeStringKey(const String& string) {
	String chars = string;
	sort(begin(chars), end(chars));
	chars.erase(unique(begin(chars), end(chars)), end(chars));
	return chars;
}

template <typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
	map<Key<String>, Group<String>> groups_map;
	for (String& string : strings) {
		groups_map[ComputeStringKey(string)].push_back(move(string));
	}
	vector<Group<String>> groups;
	for (auto&[key, group] : groups_map) {
		groups.push_back(move(group));
	}
	return groups;
}

//template <typename String>
//vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
//	// Напишите реализацию функции,
//	// использовав не более 1 копирования каждого символа
//
//	vector<Group<String>> groups;
//	vector<set<Char<String>>> test_groups;
//
//	for (auto& word : strings) {
//		bool new_gpoup = true;
//		
//		set<Char<String>> curSet;
//		for (auto c : word) {
//			curSet.insert(move(c));
//		}
//		
//		if (!test_groups.empty()) {
//			size_t count = 0;
//			for (auto& test : test_groups) {
//				if (test == curSet) {
//					groups[count].push_back(move(word));
//					new_gpoup = false;
//					break;
//				}
//				else {
//					++count;
//				}				
//			}
//		}
//
//		if (test_groups.empty() || new_gpoup) {
//			test_groups.push_back(move(curSet));
//			Group<String> g;
//			g.push_back(move(word));
//			groups.push_back(move(g));
//		}
//
//	}
//
//	return groups;
//}


void TestGroupingABC() {
	vector<string> strings = { "caab", "abc", "cccc", "bacc", "c" };
	auto groups = GroupHeavyStrings(strings);
	ASSERT_EQUAL(groups.size(), 2);
	sort(begin(groups), end(groups));  // Порядок групп не имеет значения
	ASSERT_EQUAL(groups[0], vector<string>({ "caab", "abc", "bacc" }));
	ASSERT_EQUAL(groups[1], vector<string>({ "cccc", "c" }));
}

void TestGroupingReal() {
	vector<string> strings = { "law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl" };
	auto groups = GroupHeavyStrings(strings);
	ASSERT_EQUAL(groups.size(), 4);
	sort(begin(groups), end(groups));  // Порядок групп не имеет значения
	ASSERT_EQUAL(groups[0], vector<string>({ "laptop", "paloalto" }));
	ASSERT_EQUAL(groups[1], vector<string>({ "law", "wall", "awl" }));
	ASSERT_EQUAL(groups[2], vector<string>({ "port" }));
	ASSERT_EQUAL(groups[3], vector<string>({ "top", "pot" }));
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestGroupingABC);
	RUN_TEST(tr, TestGroupingReal);

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}