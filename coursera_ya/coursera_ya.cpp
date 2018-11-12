// coursera_ya.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <algorithm>
#include <map>
#include <iomanip>

using namespace std;

struct Student {
	string firstName;
	string lastName;
	int day;
	int month;
	int year;
};

int main()
{	
	int n, m;

	cin >> n;

	vector<Student> students(n);

	for (int i = 0; i < n; i++) {
		Student& stud = students[i];
		cin >> stud.firstName >> stud.lastName >> stud.day
			>> stud.month >> stud.year;
	}

	cin >> m;

	string req;
	int num;
	for (int i = 0; i < m; i++) {
		cin >> req >> num;

		if (num <= 0 || num > n) {
			cout << "bad request" << endl;
			continue;
		}
		
		--num;

		if (req == "name") {
			cout << students[num].firstName << " " << students[num].lastName 
				<< endl;
		}
		else if (req == "date") {
			cout << students[num].day << "." << students[num].month << "." 
				<< students[num].year << endl;
		}
		else {
			cout << "bad request" << endl;
		}
	}

/*
#ifdef _MSC_VER
	system("pause");
#endif*/

	return 0;
}