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

string FindName(const map<int, string>& history, const int year)
{
	string result;
	auto it = history.upper_bound(year);
	if (it != history.begin()) {
		--it;
		result = it->second;
	}
	return result;
}

string FormationResult(const string& first_name, const string& last_name)
{
	string result;

	if (first_name.empty() && last_name.empty())
		result = "Incognito";
	else if (first_name.empty())
		result = last_name + " with unknown first name";
	else if (last_name.empty())
		result = first_name + " with unknown last name";
	else
		result = first_name + " " + last_name;

	return result;
}

class Person {
public:
	Person() = default;

	void ChangeFirstName(int year, const string& first_name) {
		history_first[year] = first_name;
	}

	void ChangeLastName(int year, const string& last_name) {
		history_last[year] = last_name;
	}

	string GetFullName(int year) const {
		// получить имя и фамилию по состоянию на конец года year

		string first_name = FindName(history_first, year);
		string last_name = FindName(history_last, year);

		return FormationResult(first_name, last_name);
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