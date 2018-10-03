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

class Person {
public:
	void ChangeFirstName(int year, const string& first_name) {
		// добавить факт изменения имени на first_name в год year
		history_first[year] = first_name;
	}
	void ChangeLastName(int year, const string& last_name) {
		// добавить факт изменения фамилии на last_name в год year
		history_last[year] = last_name;
	}
	string GetFullName(int year) {
		// получить имя и фамилию по состоянию на конец года year
		auto first = find_if(history_first.rbegin(), history_first.rend(), 
			[&year](const auto& p) {
				return p.first <= year;
			}
		);
		auto last = find_if(history_last.rbegin(), history_last.rend(),
			[&year](const auto& p) {
				return p.first <= year;
			}
		);

		if (first == history_first.rend() && last == history_last.rend()) {
			return "Incognito";
		}
		else if (first != history_first.rend() && last == history_last.rend()) {
			return first->second + " with unknown last name";
		}
		else if (first == history_first.rend() && last != history_last.rend()) {
			return last->second + " with unknown first name";
		}
		else {
			return first->second + " " + last->second;
		}
}
private:
	// приватные поля
	map<int, string> history_first;
	map<int, string> history_last;
};


int main()
{
	Person person;

	person.ChangeFirstName(1965, "Polina");
	person.ChangeLastName(1967, "Sergeeva");
	for (int year : {1900, 1965, 1990}) {
		cout << person.GetFullName(year) << endl;
	}

	person.ChangeFirstName(1970, "Appolinaria");
	for (int year : {1969, 1970}) {
		cout << person.GetFullName(year) << endl;
	}

	person.ChangeLastName(1968, "Volkova");
	for (int year : {1969, 1970}) {
		cout << person.GetFullName(year) << endl;
	}

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}