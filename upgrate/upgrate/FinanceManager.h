#pragma once

#include <map>

#include "Date.h"

class FinanceManager
{
public:
    FinanceManager();
    ~FinanceManager();

    double ComputeIncome(Date from, Date to) {
        double sum = 0;
        auto itBegin = finance.lower_bound(from);
        auto itEnd = finance.upper_bound(to);
        for (auto it = itBegin; it != itEnd; ++it) {
            sum += it->second;
        }
        return sum;
    }

    void Earn(Date from, Date to, int value) {
        int days = ComputeDaysDiff(to, from) + 1;
        double valuePerDay = static_cast<double>(value) / static_cast<double>(days);
        for (Date d = from; d <= to; ++d) {
            finance[d] += valuePerDay;
        }
    }

    void PayTax(Date from, Date to) {
        auto itBegin = finance.lower_bound(from);
        auto itEnd = finance.upper_bound(to);
        for (auto it = itBegin; it != itEnd; ++it) {
            it->second *= 0.87;
        }
    }

private:
    std::map<Date, double> finance;
};

