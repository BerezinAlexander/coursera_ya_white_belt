// coursera_ya.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
#include <cmath>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

void next_month(vector<vector<string>>& month, int& cur_num, vector<int>& months) {
	int next_num = (cur_num + 1 > 11) ? 0 : cur_num + 1;
	int count_last_days = months[cur_num] - months[next_num];
	if (count_last_days > 0) {
		// дела с лишних дней нужно переписать на последний день нового месяца
		auto& lastDayNewMonth = *next(month.rbegin(), count_last_days); // крайний месяц нового месяца
		for (int i=0; i<count_last_days; i++){
			auto lastDayCurMonth = month.back();
			lastDayNewMonth.insert(lastDayNewMonth.end(), lastDayCurMonth.begin(), lastDayCurMonth.end());
			month.pop_back();
		}
	}else{
		// добавляем новые дни
		month.insert(month.end(), abs(count_last_days), vector<string>());
	}
	cur_num = next_num;
}

void addTask(vector<vector<string>>& month, int day, string task) {
	month[day].push_back(task);
}

int main()
{
	int num_commands;
	cin >> num_commands;

	vector<int> months{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	int month_num = 0;
	vector<vector<string>> cur_month(months[month_num]);
	
	
	for (int i = 0; i < num_commands; i++) {
		string command;				
		cin >> command;
		
		if (command == "ADD") {
			int day;
			string task;
			cin >> day >> task;
			day -= 1;
			addTask(cur_month, day, task);
		}
		else if(command == "NEXT") {
			next_month(cur_month, month_num, months);
		}
		else if (command == "DUMP") {
			int day;
			cin >> day;
			day -= 1;
			cout << cur_month[day].size() << " ";
			for (auto& task : cur_month[day]) {
				cout <<  task << " ";
			}
			cout << endl;
		}	
	}

#ifdef _MSC_VER
	system("pause");
#endif

    return 0;
}

