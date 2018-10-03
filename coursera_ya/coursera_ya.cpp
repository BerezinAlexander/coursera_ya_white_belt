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

int main()
{
	int num_q;
	cin >> num_q;

	map<string, string> countries; // <country, capital>

	for (int i = 0; i < num_q; i++) {
		string command;
		cin >> command;

		if (command == "CHANGE_CAPITAL") {
			string country, capital;
			cin >> country >> capital;
			if (countries.count(country) == 0)
				cout << "Introduce new country " << country << " with capital " << capital << endl;
			else {
				if (countries[country] == capital)
					cout << "Country " << country << " hasn't changed its capital" << endl;
				else 
					cout << "Country " << country << " has changed its capital from " 
					<< countries[country] << " to " << capital << endl;
			}
			countries[country] = capital;
		}
		else if (command == "RENAME") {
			string old_country, new_country;
			cin >> old_country >> new_country;

			if (countries.count(new_country) != 0 ||
				countries.count(old_country) == 0 ||
				old_country == new_country)
			{
				cout << "Incorrect rename, skip" << endl;
			}
			else {
				cout << "Country " << old_country << " with capital " << countries[old_country] 
					<< " has been renamed to " << new_country << endl;
				countries[new_country] = countries[old_country];
				countries.erase(old_country);
			}
		}
		else if (command == "ABOUT") {
			string country;
			cin >> country;

			if (countries.count(country) == 0)
				cout << "Country " << country << " doesn't exist" << endl;
			else {
				cout << "Country " << country << " has capital " << countries[country] << endl;
			}
		}
		else if (command == "DUMP") {
			if (countries.empty())
				cout << "There are no countries in the world" << endl;
			else {
				for (auto c : countries) {
					cout << c.first << "/" << c.second << " ";
				}
				cout << endl;
			}
		}

	}


#ifdef _MSC_VER
	system("pause");
#endif

    return 0;
}

