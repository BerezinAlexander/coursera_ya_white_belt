#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <ctime>
#include <sstream>

#include "test_runner.h"

#include "Date.h"

using namespace std;

std::istream& operator >> (std::istream& input, Date& date) {
    std::string buf;
    std::getline(input, buf, '-');
    int year = stoi(buf);
    std::getline(input, buf, '-');
    int month = stoi(buf);
    int day;
    std::cin >> day;

    date = Date(year, month, day);

    return input;
}

int main() {
    //TestRunner tr;
    //RUN_TEST(tr, TestIsSubdomain);

    stringstream ss;
    ss << "1992-12-01";

    Date date(1900, 06, 05);
    ss >> date;

    int count;
    cin >> count;
   
    for (int i = 0; i < count; ++count) {
        string type;
        cin >> type;

        if (type == "ComputeIncome") {
            Date from, to;
            cin >> from >> to;
        }
        else if (type == "Earn") {

        }
        else if (type == "PayTax") {

        }
    }

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}