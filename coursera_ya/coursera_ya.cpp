// coursera_ya.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

class SortedStrings {
public:
	void AddString(const string& s) {
		strings.push_back(s);
	}
	vector<string> GetSortedStrings() {
		sort(strings.begin(), strings.end());
		return strings;
	}
private:
	vector<string> strings;
};

int main()
{

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}