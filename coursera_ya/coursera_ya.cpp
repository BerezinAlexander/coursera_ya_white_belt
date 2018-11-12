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
#include <sstream>

using namespace std;

class Rational {
public:
	Rational() : _numerator(0), _denominator(1) {};
	Rational(int numerator, int denominator) {
		if (denominator == 0)
			throw invalid_argument("");

		if (numerator == 0)
			denominator = 1;

		if (numerator < 0 && denominator < 0) {
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

bool operator==(const Rational& lhs, const Rational& rhs) {
	return (lhs.Numerator() == rhs.Numerator() && lhs.Denominator() == rhs.Denominator());
}

bool operator<(const Rational& lhs, const Rational& rhs) {
	return lhs.Numerator() * rhs.Denominator() < rhs.Numerator()*lhs.Denominator();
}

bool operator>(const Rational& lhs, const Rational& rhs) {
	return lhs.Numerator() * rhs.Denominator() > rhs.Numerator()*lhs.Denominator();
}

Rational operator+(const Rational& lhs, const Rational& rhs) {
	return Rational(lhs.Numerator() * rhs.Denominator() + rhs.Numerator()*lhs.Denominator(), lhs.Denominator() * rhs.Denominator());
}

Rational operator-(const Rational& lhs, const Rational& rhs) {
	return Rational(lhs.Numerator() * rhs.Denominator() - rhs.Numerator()*lhs.Denominator(), lhs.Denominator() * rhs.Denominator());
}

Rational operator*(const Rational& lhs, const Rational& rhs) {
	return Rational(lhs.Numerator() * rhs.Numerator(), lhs.Denominator() * rhs.Denominator());
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
	if (rhs.Numerator() == 0)
		throw domain_error("");

	return Rational(lhs.Numerator() * rhs.Denominator(), lhs.Denominator() * rhs.Numerator());
}

ostream& operator<<(ostream& stream, const Rational& rational) {
	stream << rational.Numerator() << "/" << rational.Denominator();
	return stream;
}

istream& operator >> (istream& stream, Rational& rational) {
	int n, d;
	char c;
	stream >> n >> c >> d;
	if (stream && c == '/') {
		rational = Rational(n, d);
	}
	return stream;
}

int main() {
	try {
		Rational r(1, 0);
		cout << "Doesn't throw in case of zero denominator" << endl;
		return 1;
	}
	catch (invalid_argument&) {
	}

	try {
		auto x = Rational(1, 2) / Rational(0, 1);
		cout << "Doesn't throw in case of division by zero" << endl;
		return 2;
	}
	catch (domain_error&) {
	}

	cout << "OK" << endl;

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}