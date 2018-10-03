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

string BuildVectorChangeNames(const map<int, string>& history, const int year)
{
	auto curIt = find_if(history.rbegin(), history.rend(),
		[&year](const auto& p) {
		return p.first <= year;
	}
	);

	vector<string> old_names;
	while (curIt != history.rend()) {
		if (old_names.empty())
			old_names.push_back(curIt->second);
		else {
			if (curIt->second != old_names.back())
				old_names.push_back(curIt->second);
		}
		++curIt;
	}

	string result;
	if (!old_names.empty()){
		result += old_names.front();
		if (old_names.size() > 1) {
			result += " (";
			for (int i = 1; i < old_names.size(); i++) {
				result += old_names[i];
				if(i < old_names.size() - 1)
					result += ", ";
			}
			result += ")";
		}
	}
	return result;
}

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
	string GetFullNameWithHistory(int year) {
		// получить все имена и фамилии по состоянию на конец года year
		
		string first_name = BuildVectorChangeNames(history_first, year);
		string last_name  = BuildVectorChangeNames(history_last, year);

		string result;
		if (first_name.empty() && last_name.empty()) {
			result += "Incognito";
		}
		else if (last_name.empty()) {
			result += first_name + " with unknown last name";
		}
		else if (first_name.empty()) {
			result += last_name + " with unknown first name";
		}
		else {
			result += first_name + " " + last_name;
		}
		return result;
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
		cout << person.GetFullNameWithHistory(year) << endl;
	}

	person.ChangeFirstName(1970, "Appolinaria");
	for (int year : {1969, 1970}) {
		cout << person.GetFullNameWithHistory(year) << endl;
	}

	person.ChangeLastName(1968, "Volkova");
	for (int year : {1969, 1970}) {
		cout << person.GetFullNameWithHistory(year) << endl;
	}

	person.ChangeFirstName(1990, "Polina");
	person.ChangeLastName(1990, "Volkova-Sergeeva");
	cout << person.GetFullNameWithHistory(1990) << endl;

	person.ChangeFirstName(1966, "Pauline");
	cout << person.GetFullNameWithHistory(1966) << endl;

	person.ChangeLastName(1960, "Sergeeva");
	for (int year : {1960, 1967}) {
		cout << person.GetFullNameWithHistory(year) << endl;
	}

	person.ChangeLastName(1961, "Ivanova");
	cout << person.GetFullNameWithHistory(1967) << endl;

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}