#pragma once

#include <map>

#include "Date.h"

class FinanceManager
{
public:
    FinanceManager();
    ~FinanceManager();

    double ComputeIncome(Date from, Date to) {
        double sumIncome = calcSum(finance, from, to);
        double sumExpense = calcSum(expenseBook, from, to);
        return sumIncome + sumExpense;
    }

    void Earn(Date from, Date to, int value) {
        int days = ComputeDaysDiff(to, from) + 1;
        double valuePerDay = static_cast<double>(value) / static_cast<double>(days);
        for (Date d = from; d <= to; ++d) {
            finance[d] += valuePerDay;
        }
    }

    void PayTax(Date from, Date to, int percentage) {
        auto itBegin = finance.lower_bound(from);
        auto itEnd = finance.upper_bound(to);
        for (auto it = itBegin; it != itEnd; ++it) {
            if(it->second > 0)
                it->second *= (100. - percentage) / 100.;
        }
    }

    void Spend(Date from, Date to, int value) {
        int days = ComputeDaysDiff(to, from) + 1;
        double valuePerDay = static_cast<double>(value) / static_cast<double>(days);
        for (Date d = from; d <= to; ++d) {
            expenseBook[d] -= valuePerDay;
        }
    }

private:
    double calcSum(const std::map<Date, double>& m, 
                   const Date& from, const Date& to) const 
    {
        double sum = 0;
        auto itBegin = m.lower_bound(from);
        auto itEnd = m.upper_bound(to);
        for (auto it = itBegin; it != itEnd; ++it) {
            sum += it->second;
        }
        return sum;
    }

private:
    std::map<Date, double> finance;
    std::map<Date, double> expenseBook;
};

