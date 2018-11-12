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
	
private: 
	int year;
	int month;
	int day;
};

bool operator<(const Date& lhs, const Date& rhs) {
	bool res = false;
	if (lhs.GetYear() == rhs.GetYear()) {
		if (lhs.GetMonth() == rhs.GetMonth()) {
			if (lhs.GetDay() != rhs.GetDay()) {
				res = lhs.GetDay() < rhs.GetDay();
			}
		}
		else {
			res = lhs.GetMonth() < rhs.GetMonth();
		}
	}
	else {
		res = lhs.GetYear() < rhs.GetYear();
	}
	return res;
}

bool operator==(const Date& lhs, const Date& rhs) {
	if (lhs.GetYear() == rhs.GetYear()) {
		if (lhs.GetMonth() == rhs.GetMonth()) {
			if (lhs.GetDay() == rhs.GetDay()) {
				return true;
			}
		}
	}
	return false;
}

//istream& operator>>(istream& is, Date& d) {
//	int y, m, day;
//	char ch;
//	is >> y;
//	is >> ch;
//
//}

class Database {
public:
	void AddEvent(const Date& date, const string& event);
	bool DeleteEvent(const Date& date, const string& event);
	int  DeleteDate(const Date& date) {
		if (db.count(date) > 0) {
			return db.erase(date); // delete all items!
		}
		return 0;
	}

	string Find(const Date& date) const {
		if (db.count(date) > 0) {
			return db.at(date);  
		}
		return "";
	}

	void Print() const {
		for (auto& item : db) {
			cout << item.first << " " << item.second << endl;
		}
	}
private:
	map<Date, string> db;
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

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}