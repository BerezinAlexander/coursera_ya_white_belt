#include "test_runner.h"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <ostream>
#include <string>
#include <vector>

using namespace std;


#define CAT2(a,b) a##b
#define CAT(a, b) CAT2(a,b)
#define UNIQ_ID CAT(param_, __LINE__) 

int main() {
	int UNIQ_ID = 0;
	string UNIQ_ID = "hello";
	vector<string> UNIQ_ID = { "hello", "world" };
	vector<int> UNIQ_ID = { 1, 2, 3, 4 };

#ifdef _MSC_VER
	system("pause");
#endif
}
