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

	vector<int> res;

	while (n > 1) {
		res.push_back(n % 2);
		n /= 2;
	}
	res.push_back(n);

	reverse(res.begin(), res.end());
	for (auto val : res) {
		cout << val;
	}
	cout << endl;


#ifdef _MSC_VER
	system("pause");
#endif

    return 0;
}

