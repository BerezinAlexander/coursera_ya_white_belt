// coursera_ya.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> reshetoEratosfena(int val)
{
	// init
	vector<int> arr(val + 1);
	for (int i = 0; i < val + 1; i++) {
		arr[i] = i;
	}
	arr[1] = 0;
	
	vector<int> result;
	int p = 2;
	while (p <= val) {
		if (arr[p] != 0) {
			result.push_back(arr[p]);
			for (int j = p; j < val + 1; j += p) {
				arr[j] = 0;
			}
		}
		p += 1;
	}

	return result;
}

vector<int> mnoj(vector<int>& simple, int x)
{
	vector<int> res;
	for (const auto val : simple) {
		if (val > x)
			break;
		while (x % val == 0) {
			res.push_back(val);
			x /= val;
		}
	}
	return res;
}

int main()
{
	int a, b;
	cin >> a >> b;

	while (a > 0 && b > 0)
	{
		if (a > b)
			a = a % b;
		else
			b = b % a;
	}

	cout << a + b << endl;
	/*
	if (a == b)
		cout << a << endl;
	else {
		vector<int> simple( reshetoEratosfena(max(a, b)) );

		for (auto val : simple) {
			if (a % val == 0 && b % val == 0) {
				cout << val << endl;
				break;
			}
		}

		vector<int> va = mnoj(simple, a);
		vector<int> vb = mnoj(simple, b);

		vector<int> both;
		for (auto& val : va) {
			for (auto& val2 : vb) {
				if (val == val2) {
					both.push_back(val);
					val2 = -1;
					break;
				}
			}
		}

		int mult = 1;
		for (auto val : both) {
			mult *= val;
		}

		cout << mult << endl;
	}
	*/

#ifdef _MSC_VER
	system("pause");
#endif

    return 0;
}

