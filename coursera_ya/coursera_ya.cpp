#include "test_runner.h"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <ostream>

using namespace std;


#define PRINT_VALUES(out, x, y) ((out) << (x) << endl << (y) << endl)

int main() {
	TestRunner tr;
	tr.RunTest([] {
		ostringstream output;
		PRINT_VALUES(output, 5, "red belt");
		ASSERT_EQUAL(output.str(), "5\nred belt\n");
	}, "PRINT_VALUES usage example");

#ifdef _MSC_VER
	system("pause");
#endif
}
