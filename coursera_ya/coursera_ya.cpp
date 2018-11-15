// coursera_ya.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(
	RandomIt range_begin, RandomIt range_end,
	const string& prefix)
{
	auto comp1 = [](const string& str, const string& pref) {
		if (!str.empty())
			return str.substr(0, pref.size()) < pref;
		else
			return false;
	};

	auto comp2 = [](const string& pref, const string& str) {
		if (!str.empty())
			return pref < str.substr(0, pref.size());
		else
			return false;
	};

	auto itLower = std::lower_bound(range_begin, range_end, prefix, comp1);
	auto itUpper = std::upper_bound(range_begin, range_end, prefix, comp2);

	return std::make_pair(itLower,itUpper);

	//return equal_range(range_begin, range_end, prefix, Comp);
}

int main()
{
	const vector<string> sorted_strings = { "moscow", "motovilikha", "murmansk" };

	const auto mo_result =
		FindStartsWith(begin(sorted_strings), end(sorted_strings), "mo");
	for (auto it = mo_result.first; it != mo_result.second; ++it) {
		cout << *it << " ";
	}
	cout << endl;

	const auto mt_result =
		FindStartsWith(begin(sorted_strings), end(sorted_strings), "mt");
	cout << (mt_result.first - begin(sorted_strings)) << " " <<
		(mt_result.second - begin(sorted_strings)) << endl;

	const auto na_result =
		FindStartsWith(begin(sorted_strings), end(sorted_strings), "na");
	cout << (na_result.first - begin(sorted_strings)) << " " <<
		(na_result.second - begin(sorted_strings)) << endl;

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}