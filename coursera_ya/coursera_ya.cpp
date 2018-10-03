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

int main()
{
	int num_q;
	cin >> num_q;

	set<string> uniq_str;
	for (int i = 0; i < num_q; i++) {
		string s;
		cin >> s;
		uniq_str.insert(s);
	}

	cout << uniq_str.size() << endl;


#ifdef _MSC_VER
	system("pause");
#endif

    return 0;
}

