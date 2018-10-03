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

	vector<int> arr(num_q); 

	for (int i = 0; i < num_q; i++) {
		cin >> arr[i];
	}

	sort(arr.begin(), arr.end(), 
		[] (auto val1, auto val2) {
			return abs(val1) < abs(val2);
		});

	for (auto val : arr)
		cout << val << " ";
	cout << endl;

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}