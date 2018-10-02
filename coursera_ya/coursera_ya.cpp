// coursera_ya.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
#include <cmath>
#include <set>
#include <vector>

using namespace std;

int main()
{
	//int a, b;
	//cin >> a >> b;

	//if(a == b)
	//	cout << a << endl;
	//else {

	//	vector<int> va;
	//	for (int i = 2; a != 1; i++) {
	//		while (a % i == 0) {
	//			va.push_back(i);
	//			a /= i;
	//		}
	//	}

	//	vector<int> vb;
	//	for (int i = 2; b != 1; i++) {
	//		while (b % i == 0) {
	//			vb.push_back(i);
	//			b /= i;
	//		}
	//	}

	//	vector<int> both;
	//	for (auto& val : va) {
	//		for (auto& val2 : vb) {
	//			if (val == val2) {
	//				both.push_back(val);
	//				val2 = -1;
	//				break;
	//			}
	//		}
	//	}

	//	int mult = 1;
	//	for (auto val : both) {
	//		mult *= val;
	//	}

	//	cout << mult << endl;
	//}
	
	
	string a;
	cin >> a;

	bool fl = false;
	int pos = -1;
	for (int i = 0; i < a.length(); i++) {
		if (a[i] == 'f') {
			if (!fl) {
				fl = true;
			}else{
				pos = i;
				break;
			}
		}
	}

	if(!fl)
		cout << "-2" << endl;
	else if(fl && pos == -1)
		cout << "-1" << endl;
	else
		cout << pos << endl;

	//system("pause");
    return 0;
}

