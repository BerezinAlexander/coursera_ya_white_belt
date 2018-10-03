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

map<char, int> BuildCharCounters(const string& word)
{
	map<char, int> result;
	for (char c : word) {
		result[c]++;
	}
	return result;
}

int main()
{
	int num_pair;
	cin >> num_pair;

	for (int i = 0; i < num_pair; i++) {
		string s1, s2;
		cin >> s1 >> s2;

		cout << ((BuildCharCounters(s1) == BuildCharCounters(s2)) ? "YES\n" : "NO\n");
	}


#ifdef _MSC_VER
	system("pause");
#endif

    return 0;
}

