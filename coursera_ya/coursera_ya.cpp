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

set<string> BuildMapValuesSet(const map<int, string>& m) {
	set<string> s;
	for (const auto& p : m)
		s.insert(p.second);
	return s;
}

int main()
{
	set<string> values = BuildMapValuesSet({
		{ 1, "odd" },
		{ 2, "even" },
		{ 3, "odd" },
		{ 4, "even" },
		{ 5, "odd" }
	});

	for (const string& value : values) {
		cout << value << endl;
	}


#ifdef _MSC_VER
	system("pause");
#endif

    return 0;
}

