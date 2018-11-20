#pragma once

#include <string>
using namespace std;

struct Date {
	int year, month, day;

	Date& operator=(const Date& rhs) {
		year = rhs.year;
		month = rhs.month;
		day = rhs.day;
		return *this;
	}
};

struct Time {
	int hours, minutes;

	Time& operator=(const Time& rhs) {
		hours = rhs.hours;
		minutes = rhs.minutes;
		return *this;
	}
};

struct AirlineTicket {
	string from;
	string to;
	string airline;
	Date departure_date;
	Time departure_time;
	Date arrival_date;
	Time arrival_time;
	int price;
};
