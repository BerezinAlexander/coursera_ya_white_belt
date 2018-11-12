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

class Rational {
public:
	Rational() : _numerator(0), _denominator(1) {};
	Rational(int numerator, int denominator){
		if (numerator == 0)
			denominator = 1;
		
		if (numerator < 0 && denominator < 0){ 
			numerator = abs(numerator);
			denominator = abs(denominator);
		}
		else if (denominator < 0) {
			numerator -= 2 * numerator;
			denominator = abs(denominator);
		}

		int nod = NOD(numerator, denominator);
		_numerator = numerator / nod;
		_denominator = denominator / nod;
	};

	int Numerator() const { return _numerator; };
	int Denominator() const { return _denominator; };

private:
	int _numerator, _denominator;

	int NOD(int a_, int b_) {
		// пока оба числа положительны, будем их уменьшать, не меняя их НОД
		int a = abs(a_);
		int b = abs(b_);

		while (a > 0 && b > 0) {

			// возьмём большее из чисел и заменим его остатком от деления на второе
			// действительно, если a и b делятся на x, то a - b и b делятся на x
			// тогда и a % b и b делятся на x, так что можно a заменить на a % b
			if (a > b) {
				a %= b;
			}
			else {
				b %= a;
			}

		}

		return a + b;
	}
};

int main() {
	{
		const Rational r(-3, -10);
		if (r.Numerator() != 3 || r.Denominator() != 10) {
			cout << "Rational(-3, -10) != 3/10" << endl;
			return 1;
		}
	}

	{
		const Rational r(3, 10);
		if (r.Numerator() != 3 || r.Denominator() != 10) {
			cout << "Rational(3, 10) != 3/10" << endl;
			return 1;
		}
	}

	{
		const Rational r(8, 12);
		if (r.Numerator() != 2 || r.Denominator() != 3) {
			cout << "Rational(8, 12) != 2/3" << endl;
			return 2;
		}
	}

	{
		const Rational r(-4, 6);
		if (r.Numerator() != -2 || r.Denominator() != 3) {
			cout << "Rational(-4, 6) != -2/3" << endl;
			return 3;
		}
	}

	{
		const Rational r(4, -6);
		if (r.Numerator() != -2 || r.Denominator() != 3) {
			cout << "Rational(4, -6) != -2/3" << endl;
			return 3;
		}
	}

	{
		const Rational r(0, 15);
		if (r.Numerator() != 0 || r.Denominator() != 1) {
			cout << "Rational(0, 15) != 0/1" << endl;
			return 4;
		}
	}

	{
		const Rational defaultConstructed;
		if (defaultConstructed.Numerator() != 0 || defaultConstructed.Denominator() != 1) {
			cout << "Rational() != 0/1" << endl;
			return 5;
		}
	}

	cout << "OK" << endl;


#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}