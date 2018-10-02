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

	int mid = 0;
	vector<int> vec(n);
	for (auto& v : vec) {
		cin >> v;
		mid += v;
	}
	mid /= n;

	vector<int> result;
	int count = 0;
	for (auto v : vec) {
		if (v > mid) {
			result.push_back(count);
		}
		count++;
	}

	cout << result.size() << endl;
	for (auto& v : result) {
		cout << v << " ";
	}
	cout << endl;

#ifdef _MSC_VER
	system("pause");
#endif

    return 0;
}

