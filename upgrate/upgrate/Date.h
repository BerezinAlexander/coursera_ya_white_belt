#pragma once

#include <ctime>
#include <iostream>
#include <string>

class Date {
public:
    Date()
        : year(1970), month(1), day(1)
    {}

    Date(int year_, int month_, int day_) 
        : year(year_), month(month_), day(day_) 
    {}

    bool operator < (const Date& d) const {
        return year < d.year 
            || (year == d.year && month < d.month)
            || (year == d.year && month == d.month && day < d.day);
    }

    bool operator <= (const Date& d) const {
        return year < d.year
            || (year == d.year && month < d.month)
            || (year == d.year && month == d.month && day <= d.day);
    }

    bool operator == (const Date& d) const {
        return year == d.year
            && month == d.month
            && day == d.day;
    }

    Date& operator++ () {
        if (++day > daysInMonth()) {
            day = 1;
            if (++month > 12) {
                month = 1;
                ++year;
            }
        }
        return *this;
    }

    Date operator++ (int) {
        Date d(*this);
        ++(*this);
        return d;
    }

    time_t AsTimestamp() const {
        std::tm t;
        t.tm_sec = 0;
        t.tm_min = 0;
        t.tm_hour = 0;
        t.tm_mday = day;
        t.tm_mon = month - 1;
        t.tm_year = year - 1900;
        t.tm_isdst = 0;
        return mktime(&t);
    }

private:
    int year;
    int month;
    int day;

    int daysInMonth() const {
        switch(month){

        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12: return 31;

        case 2: return isLeapYear() ? 29 : 28;

        case 4: 
        case 6: 
        case 9: 
        case 11: return 30;

        default: break;
        }
        return 0;
    }

    bool isLeapYear() const {
        return (year % 4 == 0);
    }
};

inline int ComputeDaysDiff(const Date& date_to, const Date& date_from) {
    const time_t timestamp_to = date_to.AsTimestamp();
    const time_t timestamp_from = date_from.AsTimestamp();
    static const int SECONDS_IN_DAY = 60 * 60 * 24;
    return static_cast<int>((timestamp_to - timestamp_from) / SECONDS_IN_DAY);
};