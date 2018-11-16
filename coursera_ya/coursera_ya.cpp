// coursera_ya.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include <iostream>
#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <algorithm>
#include <map>
#include <sstream>
#include <chrono>
#include <numeric>
#include <deque>

//#include "LogDuration.h"

using namespace std;
//using namespace std::chrono;

// ��������� ��������� ��� ������� ����������� ������
struct Operation {
	// ��������� �� ��������� ����� ��� ��������������� �������
	// ���������� ������� (*)
	char type = 0;
	int number = 0;
};

int main() {
	int initial_number;
	cin >> initial_number;

	int number_of_operations;
	cin >> number_of_operations;
	vector<Operation> operations(number_of_operations);  // (*)
	for (int i = 0; i < number_of_operations; ++i) {
		cin >> operations[i].type;
		cin >> operations[i].number;
	}

	deque<string> expression;
	expression.push_back(to_string(initial_number));
	for (const auto& operation : operations) {
		expression.push_front("(");
		expression.push_back(") ");
		expression.push_back(string(1, operation.type));
		expression.push_back(" ");
		expression.push_back(to_string(operation.number));
	}

	for (const string& s : expression) {
		cout << s;
	}

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}