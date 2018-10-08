// coursera_ya.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <algorithm>
#include <map>
#include <fstream>

using namespace std;

int main()
{
	ifstream input("input.txt");
	ofstream output("output.txt");

	if (input.is_open()) {
		string line;
		while (getline(input, line)) {
			//cout << line << endl;
			output << line << endl;
		}
	}

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}