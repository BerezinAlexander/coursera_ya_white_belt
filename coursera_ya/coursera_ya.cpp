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
#include <sstream>
#include <exception>

using namespace std;

// Реализуйте функции и методы классов и при необходимости добавьте свои

class Date {
public:
	Date(int year_, int month_, int day_)
		: year(year_), month(month_), day(day_)
	{}

	int GetYear() const;
	int GetMonth() const;
	int GetDay() const;
	
	bool operator<(const Date& d) const  {
		bool res = false;
		if (year == d.GetYear()) {
			if (month == d.GetMonth()) {
				if (day != d.GetDay()) {
					res = day < d.GetDay();
				}
			}
			else {
				res = month < d.GetMonth();
			}
		}
		else {
			res = year < d.GetYear();
		}
		return res;
	}

	//bool operator>(const Date& d) { return true; }
	//bool operator<=(const Date& d) { return true; }
	//bool operator>=(const Date& d) { return true; }
	bool operator==(const Date& d) const  {
		if (year == d.GetYear() && 
			month == d.GetMonth() &&
			day == d.GetDay()) 
		{
			return true;
		}
		return false;
	}
	bool operator!=(const Date& d) const {
		if (year == d.GetYear() &&
			month == d.GetMonth() &&
			day == d.GetDay())
		{
			return false;
		}
		return true;
	}

private: 
	int year;
	int month;
	int day;
};

ostream& operator<<(ostream& os, const Date& d) {
	os << d.GetYear() << "-" << d.GetMonth() << "-"
		<< d.GetDay;
	return os;
}

istream& operator>>(istream& is, Date& d) {
	int y, m, day;
	char ch;
	is >> y;
	is >> ch;
	is >> m;
	is >> ch;
	is >> day;
	return is;
}

class Database {
public:
	void AddEvent(const Date& date, const string& event) {
		db.insert({date, event});
	}

	bool DeleteEvent(const Date& date, const string& event) {
		auto it = find_if(db.begin(), db.end(), [](const auto& pair) {
			return pair.first == date && pair.second == event; });
		if (it != db.end()) {
			db.erase(it);
		}
	}

	size_t  DeleteDate(const Date& date) {
		if (db.count(date) > 0) {
			return db.erase(date); // delete all items!
		}
		return 0;
	}

	string Find(const Date& date) const {
		if (db.count(date) > 0) {
			//return db.at(date);  
		}
		return "";
	}

	void Print() const {
		for (auto& item : db) {
			//cout << item.first << " " << item.second << endl;
		}
	}
private:
	multimap<Date, string> db;
};

int main() {
	Database db;

	string command;
	while (getline(cin, command)) {
		// Считайте команды с потока ввода и обработайте каждую
		if (command == "Add") {

		}
		else if (command == "Del") {

		}
		else if (command == "Find") {

		}
		else if (command == "Print") {

		}
		else {
			cout << "Unknown command: COMMAND" << endl;
			break;
		}
	}

//#ifdef _MSC_VER
//	system("pause");
//#endif

	return 0;
}