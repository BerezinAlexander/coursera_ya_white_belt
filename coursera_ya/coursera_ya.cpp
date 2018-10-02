// coursera_ya.cpp: определяет точку входа для консольного приложения.
//

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

void calc_polynom(double a, double b, double c)
{
	if (a == 0 && b == 0) {
		//cout << "infinity roots" << endl;
		return;
	}
	if (a == 0 && b != 0) {
		cout << -c / b << endl;
	}
	else {
		float d = b * b - 4 * a * c;

		if (d < 0) {
			// no roots
			//cout << "no roots" << endl;
			return;
		}
		else {
			if (d == 0) {
				cout << -b / (2. * a) << endl;
			}
			else {
				float r1 = (-b + sqrt(d)) / (2. * a);
				cout << r1;
				float r2 = (-b - sqrt(d)) / (2. * a);
				if (r1 != r2)
					cout << " " << r2 << endl;
			}
		}
	}
}

int main()
{
	double a, b, c; // a*x^2 + b*x + c = 0

	cin >> a >> b >> c;

	calc_polynom(a,b,c);

	//calc_polynom(2, 5, 2);
	//calc_polynom(2, 4, 2);
	//calc_polynom(2, 1, 2);
	//calc_polynom(0, 4, 10);
	//calc_polynom(2, 0, 2);
	//calc_polynom(2, 4, 0);
	//calc_polynom(0, 0, 2);
	//calc_polynom(0, 4, 0);
	//calc_polynom(2, 0, 0);


	//system("pause");
    return 0;
}

