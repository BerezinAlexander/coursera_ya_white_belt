// coursera_ya.cpp: определяет точку входа для консольного приложения.
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

//#include "LogDuration.h"

using namespace std;
//using namespace std::chrono;

//void formationLine(string& line, char oper, int val) {
//	//LOG_DURATION("formationLine");
//	
//	line.insert(line.begin(), '(');
//	string str = ") " + oper + ' ' + to_string(val);
//	line.insert(line.length(), str);
//	
//	//line.reserve(line.length() * 2);
//
//	//line.insert(line.begin(), '(');
//	//line.insert(line.length(), ") ");
//	//line.insert(line.length(), 1, oper);
//	//line.insert(line.length(), 1, ' ');
//	//line.insert(line.length(), to_string(val));
//}

int main()
{
	int val, n;
	cin >> val >> n;

	char c;
	int cur_val;
	size_t count = 0;

	string post = ") + ";
	string post2 = " + ";
	string str = to_string(val);

	bool need = false;

	for (int i = 0; i < n; ++i) {
		cin >> c >> cur_val;		

		if (need && (c == '*' || c == '/')) {
			need = false;
			++count;
			post[2] = c;
			str.insert(str.length(), post);
		}
		else {
			if(c == '-' || c == '+')
				need = true;
			post2[1] = c;
			str.insert(str.length(), post2);
		}
		str.insert(str.length(), to_string(cur_val));
	}
	
	str.insert(str.begin(), count, '(');

	cout << str << endl;
	
	
	
	//int val = 2;

	////cin >> val >> n;

	//const size_t count_oper = 50;
	//stringstream ss2;
	//for (int i = 0; i < count_oper; i++) {
	//	ss2 << " + 7";
	//}
	//string ideal = ss2.str();

	//vector<long long> times;
	//steady_clock::time_point start;
	//
	//const int max_count = 1000;
	//for (int j = 0; j < max_count; j++) {

	//	stringstream ss;
	//	ss << ideal;   //"+ 1 - 2 * 3 / 4 + 5 / 6 - 7 * 7 + 4 - 3 * 5 / 6 * 7 - 8 - 9";


	//	{
	//		start = steady_clock::now();

	//		string str = to_string(val);
	//		char c;
	//		int cur_val;
	//		size_t count = 0;
	//		string post = ") + ";
	//		for (int i = 0; i < count_oper; ++i) {
	//			ss >> c >> cur_val;
	//			post[2] = c;
	//			++count;
	//			str.insert(str.length(), post);
	//			str.insert(str.length(), to_string(cur_val));
	//		}

	//		str.insert(str.begin(), count, '(');


	//		times.emplace_back(duration_cast<microseconds>(steady_clock::now() - start).count());

	//		cout << str << endl;
	//	}
	//}

	////duration_cast<microseconds>(dur).count()

	//long long sum = accumulate(times.begin(), times.end(), 0);
	//int average = sum / times.size();

	//cout << "Average (" << max_count << "): " << average << " microseconds" << endl;

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}