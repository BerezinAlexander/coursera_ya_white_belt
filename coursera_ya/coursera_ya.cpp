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
		: year(year_)
	{
		if (month_ < 1 || month_ > 12)
			throw invalid_argument("Month value is invalid: " + to_string(month_));

		if (day_ < 1 || day_ > 31)
			throw invalid_argument("Day value is invalid: " + to_string(day_));
		
		month = month_;
		day = day_;
	}

	Date(const Date& d) 
		: year(d.GetYear()), month(d.GetMonth()), day(d.GetDay())
	{}

	int GetYear() const { return year; }
	int GetMonth() const { return month; }
	int GetDay() const { return day; }
	
	bool operator<(const Date& d) const  {
		bool res = false;
		if (year != d.GetYear()) {
			res = year < d.GetYear();
		}
		else if (month != d.GetMonth()) {
			res = month < d.GetMonth();
		}
		else if(day != d.GetDay()) {
			res = day < d.GetDay();
		}
		else {
			return false;
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

	Date& operator=(const Date& d) {
		year = d.GetYear();
		month = d.GetMonth();
		day = d.GetDay();
		return *this;
	}

private: 
	int year;
	int month;
	int day;
};

ostream& operator<<(ostream& os, const Date& d) {
	os << setw(4) << setfill('0') << d.GetYear() 
		<< "-" << setw(2) << d.GetMonth() 
		<< "-" << setw(2) << d.GetDay();
	return os;
}

istream& operator>>(istream& is, Date& d) {
	stringstream ss;
	string line;
	is >> line;

	string err = "Wrong date format: " + line;
	if(line.empty())
		throw runtime_error(err);

	ss << line;
	int year = -1, month = -1, day = -1;
	ss >> year;
	if(ss.peek() != '-')
		throw runtime_error(err);
	ss.ignore(1);
	ss >> month;
	if (ss.peek() != '-')
		throw runtime_error(err);
	ss.ignore(1);
	if(!( ss >> day))
		throw runtime_error(err);

	if (!ss.eof())
		throw runtime_error(err);

	d = Date(year, month, day);

	return is;
}

class Database {
public:
	void AddEvent(const Date& date, const string& event) {
		if (!event.empty()) {
			auto it = find_if(db.begin(), db.end(), [&date, &event](const auto& pair) {
				return pair.first == date && pair.second == event; });
			if (it == db.end())
				db.emplace(date, event);
		}
	}

	bool DeleteEvent(const Date& date, const string& event) {
		if (!event.empty()) {
			auto it = find_if(db.begin(), db.end(), [&date, &event](const auto& pair) {
				return pair.first == date && pair.second == event; });
			if (it != db.end()) {
				db.erase(it);
				return true;
			}
		}
		return false;
	}

	size_t DeleteDate(const Date& date) {
		size_t count = db.count(date);
		if (count > 0) {
			auto it = db.begin();
			while (true) {
				it = find_if(it, db.end(), [&date](const auto& pair) {
					return pair.first == date;});
				if (it == db.end())
					break;
				it = db.erase(it);
			}
		}
		return count;
	}

	string Find(const Date& date) const {
		string text = "";
		size_t count = db.count(date);
		if (count > 0) {
			auto it = find_if(db.begin(), db.end(), [&date](const auto& pair) {
				return pair.first == date;
			});
			if (it != db.end()) {
				set<string> s;
				s.insert(it->second);
				//text = it->second;
				for (auto itDate = next(it); itDate != next(it, count); ++itDate) {
					s.insert(itDate->second);
					//text += '\n' + itDate->second;
				}
				for (auto& ev : s) {
					cout << ev << endl;
				}
			}
		}
		return text;
	}

	void Print() const {
		map<Date, set<string>> ms;
		for (auto& item : db) {
			ms[item.first].insert(item.second);
			//cout << item.first << " " << item.second << endl;
		}
		for (auto& item : ms) {
			for (auto& str : item.second) {
				cout << item.first << " " << str << endl;
			}
		}
	}
private:
	multimap<Date, string> db;
};

int main() {
	try {
		Database db;

		string command;
		Date date(1, 1, 1);
		string event = "";
		while (getline(cin, command)) {
			// Считайте команды с потока ввода и обработайте каждую
			if (command.empty() || command == " ")
				continue;

			stringstream ss;
			ss << command;

			string com;
			ss >> com;


			//string com = command;
			//size_t pos_beg = 0;
			//size_t pos_end = command.find(' ');
			//if(pos_end == string::npos)
			//	com = command.substr(pos_beg, pos_end);

			if (com == "Add") {
				try {
					event = "";
					
					// парсинг даты
					string sdata;
					ss >> sdata;
					stringstream ss2;
					ss2 << sdata;
					ss2 >> date;
					
					ss >> event;

					if (!(event.empty() || event == " "))
						db.AddEvent(date, event);
					else
						throw runtime_error("");
				}
				catch (invalid_argument& er) {
					cout << er.what() << endl;
					break;
				}
				catch (...) {
					stringstream ss;
					ss << command;
					string com;
					ss >> com;
					string sdate;
					ss >> sdate;
					cout << "Wrong date format: " << sdate << endl;
					break;
				}
			}
			else if (com == "Del") {
				try {
					event = "";

					// парсинг даты
					string sdata;
					ss >> sdata;
					stringstream ss2;
					ss2 << sdata;
					ss2 >> date;

					ss >> event;
					if (event.empty()) {
						int num = db.DeleteDate(date);
						cout << "Deleted " << num << " events" << endl;
					}
					else {
						if (db.DeleteEvent(date, event))
							cout << "Deleted successfully" << endl;
						else
							cout << "Event not found" << endl;
					}
				}
				catch (...) {
					stringstream ss;
					ss << command;
					string com;
					ss >> com;
					string sdate;
					ss >> sdate;
					cout << "Wrong date format: " << sdate << endl;
					break;
				}
			}
			else if (com == "Find") {
				try {
					// парсинг даты
					string sdata;
					ss >> sdata;
					stringstream ss2;
					ss2 << sdata;
					ss2 >> date;

					db.Find(date);
					//cout << db.Find(date) << endl;
				}
				catch (...) {
					stringstream ss;
					ss << command;
					string com;
					ss >> com;
					string sdate;
					ss >> sdate;
					cout << "Wrong date format: " << sdate << endl;
					break;
				}
			}
			else if (com == "Print") {
				db.Print();
			}
			else {
				cout << "Unknown command: " << com << endl;
				return 0;
			}
		}
	}
	catch (...) {

	}

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}