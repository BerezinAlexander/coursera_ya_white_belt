// coursera_ya.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

bool IsPalindrom(string text)
{
	if (text.empty())
		return true;

	int middle = text.size() / 2;
	for (size_t i = 0; i < middle; i++)
	{
		if (text[i] != text[text.size() - i - 1])
			return false;
	}

	return true;
}

vector<string> PalindromFilter(vector<string> words, int minLength)
{
	vector<string> result;
	for (auto& word : words) {
		if (IsPalindrom(word) && word.size() >= minLength) {
			result.push_back(word);
		}
	}
	return result;
}

//int main()
//{
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

