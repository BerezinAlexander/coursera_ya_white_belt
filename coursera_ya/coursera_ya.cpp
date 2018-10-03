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
	int num_q;
	cin >> num_q;

	map<string, set<string>> m;

	for (int i = 0; i < num_q; i++) {
		string command;
		cin >> command;

		if (command == "ADD") {
			string word1, word2;
			cin >> word1 >> word2;

			m[word1].insert(word2);
			m[word2].insert(word1);
		}
		else if (command == "COUNT") {
			string word;
			cin >> word;

			cout << m[word].size() << endl;
		}
		else if (command == "CHECK") {
			string word1, word2;
			cin >> word1 >> word2;

			if (m[word1].count(word2) == 0)
				cout << "NO";
			else
				cout << "YES";
			cout << endl;
		}
	}

#ifdef _MSC_VER
	system("pause");
#endif

    return 0;
}

