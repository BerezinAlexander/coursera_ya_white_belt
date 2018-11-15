// coursera_ya.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include <iostream>
#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

map<int, string>::iterator BinarySearcher(map<int, string>::iterator& itBeg, 
	map<int, string>::iterator& itEnd, const int year) 
{
	auto itFind = next(itBeg, (distance(itBeg, itEnd)) / 2);
	if()
}

string FindName(const map<int, string>& history, const int year)
{
	// binary finder
	auto itFind = next(history.begin(), history.size() / 2);


	// basic finder
	//auto rIt = find_if( history.rbegin(), history.rend(),
	//					[&year](const auto& p) {
	//						return p.first <= year;
	//					}
	//);
	//string result;
	//if (rIt != history.rend())
	//	result = rIt->second;


	return result;
}

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
	if (!old_names.empty()) {
		result += old_names.front();
		if (old_names.size() > 1) {
			result += " (";
			for (int i = 1; i < old_names.size(); i++) {
				result += old_names[i];
				if (i < old_names.size() - 1)
					result += ", ";
			}
			result += ")";
		}
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
	Person(const string& fname, const string& lname, const int year) 
		: birthYear(year)
	{
		history_first[year] = fname;
		history_last[year] = lname;
	}

	void ChangeFirstName(int year, const string& first_name) {
		// �������� ���� ��������� ����� �� first_name � ��� year
		if (year >= birthYear) {
			history_first[year] = first_name;
		}
	}
	void ChangeLastName(int year, const string& last_name) {
		// �������� ���� ��������� ������� �� last_name � ��� year
		if (year >= birthYear) {
			history_last[year] = last_name;
		}
	}
	string GetFullName(int year) const {
		// �������� ��� � ������� �� ��������� �� ����� ���� year

		if (year < birthYear) {
			return "No person";
		}

		string first_name = FindName(history_first, year);
		string last_name = FindName(history_last, year);

		return FormationResult(first_name, last_name);
	}
	string GetFullNameWithHistory(int year) const {
		// �������� ��� ����� � ������� �� ��������� �� ����� ���� year

		if (year < birthYear) {
			return "No person";
		}

		string first_name = BuildVectorChangeNames(history_first, year);
		string last_name = BuildVectorChangeNames(history_last, year);

		return FormationResult(first_name, last_name);
	}

private:
	// ��������� ����
	int birthYear;
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

//#ifdef _MSC_VER
//	system("pause");
//#endif

	return 0;
}