#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "test_runner.h"

#include "Date.h"
#include "FinanceManager.h"

using namespace std;

std::istream& operator >> (std::istream& input, Date& date) {
    std::string buf;
    std::getline(input, buf, '-');
    int year = stoi(buf);
    std::getline(input, buf, '-');
    int month = stoi(buf);
    int day;
    input >> day;

    date = Date(year, month, day);

    return input;
}

void Process(istream& input, ostream& output) {

    int count;
    input >> count;

    FinanceManager fm;

    for (int i = 0; i < count; ++i) {
        string type;
        input >> type;

        if (type == "ComputeIncome") {
            Date from, to;
            input >> from >> to;
            double income = fm.ComputeIncome(from, to);
            output << fixed << income << endl;
        }
        else if (type == "Earn") {
            Date from, to;
            int value = 0;
            input >> from >> to >> value;
            fm.Earn(from, to, value);
        }
        else if (type == "PayTax") {
            Date from, to;
            int percentage = 0;
            input >> from >> to >> percentage;
            fm.PayTax(from, to, percentage);
        }
        else if (type == "Spend") {
            Date from, to;
            int value = 0;
            input >> from >> to >> value;
            fm.Spend(from, to, value);
        }
    }
}

inline bool compareStrings(istream& output, istream& idealOutput) {
    vector<double> values;
    double value = 0;
    while (output >> value) {
        values.emplace_back(value);
    }

    vector<double> idealValues;
    while (idealOutput >> value) {
        idealValues.emplace_back(value);
    }

    if (values.size() != idealValues.size())
        return false;

    for (size_t i = 0; i < values.size(); ++i) {
        ASSERT_EQUAL(abs(values[i] - idealValues[i]) < 0.0001, true);
    }

    return true;
}

void TestDataLess() {
    ASSERT_EQUAL(Date(2000, 1, 1) < Date(2000, 1, 2), true);
    ASSERT_EQUAL(Date(2000, 1, 1) < Date(2000, 2, 1), true);
    ASSERT_EQUAL(Date(2000, 1, 2) < Date(2000, 2, 1), true);
}

void Test1() {
    string sInput = "\
2\n\
Earn 2000-01-01 2000-01-01 20\n\
ComputeIncome 2000-01-01 2001-01-01\n";

    stringstream input(sInput);

    stringstream output;

    Process(input, output);

    string sIdeal = "\
20\n";
    stringstream ssIdeal;
    ssIdeal << sIdeal;
    
    ASSERT_EQUAL(compareStrings(output, ssIdeal), true);
}

void Test2() {
    string sInput = "\
2\n\
Earn 2000-01-02 2000-01-06 20\n\
ComputeIncome 2000-01-02 2000-01-02\n";

    stringstream input(sInput);

    stringstream output;

    Process(input, output);

    string sIdeal = "\
4\n";
    stringstream ssIdeal;
    ssIdeal << sIdeal;

    ASSERT_EQUAL(compareStrings(output, ssIdeal), true);
}

void Test3() {
    string sInput = "\
3\n\
Earn 2000-01-02 2000-04-21 1000000\n\
PayTax 2000-01-02 2000-05-03 13\n\
ComputeIncome 2000-01-01 2001-01-01\n";

    stringstream input(sInput);

    stringstream output;

    Process(input, output);

    string sIdeal = "\
870000\n";
    stringstream ssIdeal;
    ssIdeal << sIdeal;

    ASSERT_EQUAL(compareStrings(output, ssIdeal), true);
}


void TestMain() {
    string sInput = "\
8\n\
Earn 2000-01-02 2000-01-06 20\n\
ComputeIncome 2000-01-01 2001-01-01\n\
PayTax 2000-01-02 2000-01-03 13\n\
ComputeIncome 2000-01-01 2001-01-01\n\
Earn 2000-01-03 2000-01-03 10\n\
ComputeIncome 2000-01-01 2001-01-01\n\
PayTax 2000-01-03 2000-01-03 13\n\
ComputeIncome 2000-01-01 2001-01-01\n";

    stringstream input(sInput);

    stringstream output;

    Process(input, output);

    string sIdeal = "\
20\n\
18.96\n\
28.96\n\
27.2076\n";
    stringstream ssIdeal;
    ssIdeal << sIdeal;

    ASSERT_EQUAL(compareStrings(output, ssIdeal), true);
}

void TestMain2() {
    string sInput = "\
8\n\
Earn 2000-01-02 2000-01-06 20\n\
ComputeIncome 2000-01-01 2001-01-01\n\
PayTax 2000-01-02 2000-01-03 13\n\
ComputeIncome 2000-01-01 2001-01-01\n\
Spend 2000-12-30 2001-01-02 14\n\
ComputeIncome 2000-01-01 2001-01-01\n\
PayTax 2000-12-30 2000-12-30 13\n\
ComputeIncome 2000-01-01 2001-01-01\n";

    stringstream input(sInput);

    stringstream output;

    Process(input, output);

    string sIdeal = "\
20\n\
18.96\n\
8.46\n\
8.46\n";
    stringstream ssIdeal;
    ssIdeal << sIdeal;

    ASSERT_EQUAL(compareStrings(output, ssIdeal), true);
}

int main() {

#ifdef _MSC_VER
    TestRunner tr;
    RUN_TEST(tr, TestDataLess);
    RUN_TEST(tr, Test1);
    RUN_TEST(tr, Test2);
    RUN_TEST(tr, Test3);
    RUN_TEST(tr, TestMain);
    RUN_TEST(tr, TestMain2);

	system("pause");
#else
    Process(cin, cout);
#endif

	return 0;
}