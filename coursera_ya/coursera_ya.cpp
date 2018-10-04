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

struct Specialization
{
	string str;

	explicit Specialization(const string& s)
		: str(s)
	{}

	string Value() const {
		return str;
	}
};

struct Course
{
	string str;

	explicit Course(const string& c)
		: str(c)
	{}
	string Value() const {
		return str;
	}
};

struct Week
{
	string str;

	explicit Week(const string& w){
		str = w;
	}
	string Value() const {
		return str;
	}
};

struct LectureTitle {
	string specialization;
	string course;
	string week;

	LectureTitle(const Specialization& sp, const Course& c, const Week& w)
		: specialization(sp.Value()), course(c.Value()), week(w.Value())
	{
	}
};

int main()
{
	LectureTitle title(
		Specialization("C++"),
		Course("White belt"),
		Week("4th")
	);

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}