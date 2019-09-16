#pragma once

#include <ctime>
#include <iostream>
#include <string>

class Date {
public:
    Date()
        : year(1900), month(1), day(1)
    {}

    Date(int year_, int month_, int day_) 
        : year(year_), month(month_), day(day_) 
    {}

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
};

int ComputeDaysDiff(const Date& date_to, const Date& date_from) {
    const time_t timestamp_to = date_to.AsTimestamp();
    const time_t timestamp_from = date_from.AsTimestamp();
    static const int SECONDS_IN_DAY = 60 * 60 * 24;
    return (timestamp_to - timestamp_from) / SECONDS_IN_DAY;
};