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
#include <memory>
#include <iomanip>

////#include "LogDuration.h"

using namespace std;
//using namespace std::chrono;

class Figure {
public:
	virtual string Name() = 0;
	virtual double Perimeter() = 0;
	virtual double Area() = 0;
};

class Triangle : public Figure {
public:
	Triangle(int a_, int b_, int c_)
		: a(a_), b(b_), c(c_)
	{}
	string Name() override {
		return "TRIANGLE";
	}
	double Perimeter() override {
		return a + b + c;
	}
	double Area() override {
		double p = Perimeter() / 2;
		return sqrt(p*(p - a)*(p - b)*(p - c));
	}
private:
	int a, b, c;
};

class Rect : public Figure {
public:
	Rect(int a_, int b_)
		: a(a_), b(b_)
	{}
	string Name() override {
		return "RECT";
	}
	double Perimeter() override {
		return 2 * a + 2 * b;
	}
	double Area() override {
		return a*b;
	}
private:
	int a, b;
};

class Circle : public Figure {
public:
	Circle(int r_)
		: r(r_)
	{}
	string Name() override {
		return "CIRCLE";
	}
	double Perimeter() override {
		return 2.*3.14*r;
	}
	double Area() override {
		return 3.14*r*r;
	}
private:
	int r;
};

shared_ptr<Figure> CreateFigure(istream& is) {
	string type;
	is >> type;
	if (type == "TRIANGLE") {
		int a, b, c;
		is >> a >> b >> c;
		return shared_ptr<Figure>(new Triangle(a, b, c));
	}
	else if (type == "RECT") {
		int a, b;
		is >> a >> b;
		return shared_ptr<Figure>(new Rect(a, b));
	}
	else if (type == "CIRCLE") {
		int a;
		is >> a;
		return shared_ptr<Figure>(new Circle(a));
	}

	return shared_ptr<Figure>();
}

int main() {
	vector<shared_ptr<Figure>> figures;
	for (string line; getline(cin, line); ) {
		istringstream is(line);

		string command;
		is >> command;
		if (command == "ADD") {
			figures.push_back(CreateFigure(is));
		}
		else if (command == "PRINT") {
			for (const auto& current_figure : figures) {
				cout << fixed << setprecision(3)
					<< current_figure->Name() << " "
					<< current_figure->Perimeter() << " "
					<< current_figure->Area() << endl;
			}
		}
	}

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}