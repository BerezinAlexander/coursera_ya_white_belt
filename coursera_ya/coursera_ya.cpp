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

	int count_bus = 0;
	map<vector<string>, int> buses; // <stops, num_bus>

	for (int i = 0; i < num_q; i++) {
		int count;
		cin >> count;

		vector<string> stops(count);
		for (int i = 0; i < count; i++) {
			cin >> stops[i];
		}

		if (buses.count(stops) == 0) {
			buses[stops] = ++count_bus;
			cout << "New bus ";
		}
		else {
			cout << "Already exists for ";
		}
		cout << buses[stops] << endl;
	}


#ifdef _MSC_VER
	system("pause");
#endif

    return 0;
}

