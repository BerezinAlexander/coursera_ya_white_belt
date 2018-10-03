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

#include <algorithm>
#include <map>
#include <vector>

using namespace std;

class ReversibleString {
public:
	ReversibleString() = default;
	ReversibleString(const string& str) 
		: data(str) 
	{
	}

	void Reverse() {
		reverse(data.begin(), data.end());
	}

	string ToString() const {
		return data;
	}

private:
	string data;
};

int main()
{
	ReversibleString s("live");
	s.Reverse();
	cout << s.ToString() << endl;

	s.Reverse();
	const ReversibleString& s_ref = s;
	string tmp = s_ref.ToString();
	cout << tmp << endl;

	ReversibleString empty;
	cout << '"' << empty.ToString() << '"' << endl;

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}