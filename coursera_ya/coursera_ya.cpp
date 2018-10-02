// coursera_ya.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

void Reverse(vector<int>& v)
{
	reverse(v.begin(), v.end());
}

//int main()
//{
//	const vector<int> numbers = { 1, 2, 3, 4, 5 };
//	for (auto x : numbers) { cout << x << " "; }
//
//	string text;
//	cin >> text;
//
//	cout << boolalpha << IsPalindrom(text) << endl;
//
//
//#ifdef _MSC_VER
//	system("pause");
//#endif
//
//    return 0;
//}
//
