#include "airline_ticket.h"
#include "test_runner.h"

#include <algorithm>
#include <numeric>
using namespace std;

bool operator<(const Date& lhs, const Date& rhs) {
	return vector<int>{lhs.year, lhs.month, lhs.day} <
		vector<int>{rhs.year, rhs.month, rhs.day};
}

bool operator==(const Date& lhs, const Date& rhs) {
	return vector<int>{lhs.year, lhs.month, lhs.day} ==
		vector<int>{rhs.year, rhs.month, rhs.day};
}

ostream& operator<<(ostream& os, const Date& date) {
	os << date.year << "-" << date.month << "-" << date.day;
	return os;
}

ostream& operator<<(ostream& os, const Time& date) {
	os << date.hours << ":" << date.minutes;
	return os;
}

bool operator<(const Time& lhs, const Time& rhs) {
	return vector<int>{lhs.hours, lhs.minutes} <
		vector<int>{rhs.hours, rhs.minutes};
}

bool operator==(const Time& lhs, const Time& rhs) {
	return vector<int>{lhs.hours, lhs.minutes} ==
		vector<int>{rhs.hours, rhs.minutes};
}

// Реализуйте этот макрос, а также необходимые операторы для классов Date и Time
#define SORT_BY(field) 											\
	[](const AirlineTicket& lhs, const AirlineTicket& rhs) {	\
		return lhs.field < rhs.field;							\
	}		

#define EQUAL_BY(field) 					\
	[](const pair<string, string>& p) {		\
		return p.first == #field;			\
	}	

// Реализуйте этот макрос, а также необходимые операторы для классов Date и Time
#define UPDATE_FIELD(ticket, field, values) {									\
	auto it = find_if(values.begin(), values.end(), EQUAL_BY(field));			\
	if (it != values.end())														\
		ticket.field = it->second;	\
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
	RUN_TEST(tr, TestSortBy);

#ifdef _MSC_VER
	system("pause");
#endif
}
