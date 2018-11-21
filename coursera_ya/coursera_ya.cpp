#include "airline_ticket.h"
#include "test_runner.h"

#include <algorithm>
#include <numeric>
using namespace std;


Date ParseDate(std::istream& is)
{
	string date;
	is >> date;
	istringstream date_stream(date);
	bool ok = true;

	int year;
	ok = ok && (date_stream >> year);
	ok = ok && (date_stream.peek() == '-');
	date_stream.ignore(1);

	int month;
	ok = ok && (date_stream >> month);
	ok = ok && (date_stream.peek() == '-');
	date_stream.ignore(1);

	int day;
	ok = ok && (date_stream >> day);
	ok = ok && date_stream.eof();

	if (!ok) {
		throw logic_error("Wrong date format: " + date);
	}
	return{ year, month, day };
}

Time ParseTime(std::istream& is)
{
	string stime;
	is >> stime;
	istringstream time_stream(stime);
	bool ok = true;

	int hours;
	ok = ok && (time_stream >> hours);
	ok = ok && (time_stream.peek() == ':');
	time_stream.ignore(1);

	int minutes;
	ok = ok && (time_stream >> minutes);
	ok = ok && time_stream.eof();

	if (!ok) {
		throw logic_error("Wrong time format: " + stime);
	}
	return{ hours, minutes };
}

template<class T, class Func>
T ParseFromStr(const std::string& s) {
	stringstream ss;
	ss << s;
	try {
		return Func(ss);
	}
	catch (...) {
		return T();
	}
}


bool operator<(const Date& lhs, const Date& rhs) {
	return vector<int>{lhs.year, lhs.month, lhs.day} <
		vector<int>{rhs.year, rhs.month, rhs.day};
}
bool operator==(const Date& lhs, const Date& rhs) {
	return vector<int>{lhs.year, lhs.month, lhs.day} ==
		vector<int>{rhs.year, rhs.month, rhs.day};
}
bool operator!=(const Date& lhs, const Date& rhs) {
	return vector<int>{lhs.year, lhs.month, lhs.day} !=
		vector<int>{rhs.year, rhs.month, rhs.day};
}
ostream& operator<<(ostream& os, const Date& date) {
	os << date.year << "-" << date.month << "-" << date.day;
	return os;
}
istream& operator>>(istream& is, Date& date) {
	try {
		date = ParseDate(is);
	}
	catch (...) {
	}
	return is;
}


bool operator<(const Time& lhs, const Time& rhs) {
	return vector<int>{lhs.hours, lhs.minutes} <
		vector<int>{rhs.hours, rhs.minutes};
}
bool operator==(const Time& lhs, const Time& rhs) {
	return vector<int>{lhs.hours, lhs.minutes} ==
		vector<int>{rhs.hours, rhs.minutes};
}
bool operator!=(const Time& lhs, const Time& rhs) {
	return vector<int>{lhs.hours, lhs.minutes} !=
		vector<int>{rhs.hours, rhs.minutes};
}
ostream& operator<<(ostream& os, const Time& date) {
	os << date.hours << ":" << date.minutes;
	return os;
}
istream& operator >> (istream& is, Time& time) {
	try {
		time = ParseTime(is);
	}
	catch (...) {
	}
	return is;
}


// Реализуйте этот макрос, а также необходимые операторы для классов Date и Time
#define SORT_BY(field) 											\
	[](const AirlineTicket& lhs, const AirlineTicket& rhs) {	\
		return lhs.field < rhs.field;							\
	}		

//#define EQUAL_BY(field) 					\
//	[](const pair<string, string>& p) {		\
//		return p.first == #field;			\
//	}	

// Реализуйте этот макрос, а также необходимые операторы для классов Date и Time
#define UPDATE_FIELD(ticket, field, values) {									\
	auto it = values.find(#field);	\
	if (it != values.end()) {	\
		istringstream is(it->second);	\
		is >> ticket.field;	\
	}	\
}

void TestUpdate() {
	AirlineTicket t;
	t.price = 0;


	const map<string, string> updates1 = {
		{ "departure_date", "2018-2-28" },
		{ "departure_time", "17:40" },
	};

	UPDATE_FIELD(t, departure_date, updates1);
	UPDATE_FIELD(t, departure_time, updates1);
	UPDATE_FIELD(t, price, updates1);

	ASSERT_EQUAL(t.departure_date, (Date{ 2018, 2, 28 }));
	ASSERT_EQUAL(t.departure_time, (Time{ 17, 40 }));
	ASSERT_EQUAL(t.price, 0);

	const map<string, string> updates2 = {
		{ "price", "12550" },
		{ "arrival_time", "20:33" },
	};
	UPDATE_FIELD(t, departure_date, updates2);
	UPDATE_FIELD(t, departure_time, updates2);
	UPDATE_FIELD(t, arrival_time, updates2);
	UPDATE_FIELD(t, price, updates2);

	// updates2 не содержит ключей "departure_date" и "departure_time", поэтому
	// значения этих полей не должны измениться
	ASSERT_EQUAL(t.departure_date, (Date{ 2018, 2, 28 }));
	ASSERT_EQUAL(t.departure_time, (Time{ 17, 40 }));
	ASSERT_EQUAL(t.price, 12550);
	ASSERT_EQUAL(t.arrival_time, (Time{ 20, 33 }));
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestUpdate);

#ifdef _MSC_VER
	system("pause");
#endif
}
