// coursera_ya.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
	int n;
	cin >> n;

	vector<int> q;
	for (int i = 0; i < n; i++) {
		string command;
		int num;		
		cin >> command;
		
		if (command == "WORRY") {
			cin >> num;
			q[num] = 1;
		}
		else if(command == "QUIET") {
			cin >> num;
			q[num] = 0;
		}
		else if (command == "COME") {
			cin >> num;
			if (num < 0) {
				for (int j = 0; j < abs(num); j++)
					q.pop_back();
			}
			else {
				for (int j = 0; j < abs(num); j++)
					q.push_back(0);
			}
		}
		else if (command == "WORRY_COUNT") {
			int crazy = 0;
			for (auto state : q)
				if (state == 1)
					crazy++;
			cout << crazy << endl;
		}
	}

#ifdef _MSC_VER
	system("pause");
#endif

    return 0;
}

