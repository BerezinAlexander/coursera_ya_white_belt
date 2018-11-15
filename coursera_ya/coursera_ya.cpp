// coursera_ya.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <algorithm>
#include <map>
#include <sstream>

#include "LogDuration.h"

using namespace std;

void formationLine(string& line, char oper, int val) {
	//LOG_DURATION("formationLine");
	line.insert(line.begin(), '(');
	line.insert(line.length(), ") ");
	line.insert(line.length(), 1, oper);
	line.insert(line.length(), 1, ' ');
	line.insert(line.length(), to_string(val));
	//line.reserve(line.length() * 2);
}

int main()
{
	int val = 2, n = 8;

	//cin >> val >> n;

	for (int j = 0; j < 50; j++) {

		stringstream ss;
		ss << "+ 1 - 2 * 3 / 4 + 5 / 6 - 7 * 7 + 4 - 3 * 5 / 6 * 7 - 8 - 9";

		{
			LOG_DURATION("main");

			string str = to_string(val);
			for (int i = 0; i < n; ++i) {
				char c;
				int cur_val;
				ss >> c >> cur_val;
				formationLine(str, c, cur_val);
			}

			//cout << str << endl;
		}
	}

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}