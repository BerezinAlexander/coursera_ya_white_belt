// coursera_ya.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <algorithm>
#include <map>
#include <iomanip>

using namespace std;

int main()
{
	ifstream input("input.txt");

	if (!input) {
		//cout << "File not open!" << endl;
		return 0;
	}

	int n, m;

	input >> n >> m;

	vector<vector<int>> table;
	table.resize(n);
	for (auto& line : table) {
		line.resize(m);
	}

	auto itLine = table.begin();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			input >> (*itLine)[j];
			if(j < m-1)
				input.ignore(1);
		}
		itLine++;
	}

	for (auto it = table.begin(); it != table.end(); it++) {
		for (auto itItem = it->begin(); itItem != it->end(); itItem++) {
			if (itItem != it->begin())
				cout << " ";
			cout << setw(10) << *itItem;
		}
		if(it < prev( table.end() ) )
			cout << endl;
	}
/*
#ifdef _MSC_VER
	system("pause");
#endif*/

	return 0;
}