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

class FunctionParts {
public:
	FunctionParts(char new_operation, double new_value)
		: operation(new_operation), value(new_value)
	{}
	double Apply(double source_value) const {
		if (operation == '+') {
			return source_value + value;
		}
		else if (operation == '-') {
			return source_value - value;
		}
		else if (operation == '*') {
			return source_value * value;
		}
		else if (operation == '/') {
			return source_value / value;
		}
	}
	void Invert() {
		if (operation == '+') {
			operation = '-';
		}
		else if (operation == '-') {
			operation = '+';
		}
		else if (operation == '*') {
			operation = '/';
		}
		else if (operation == '/') {
			operation = '*';
		}
	}
private:
	char operation;
	double value;
};

class Function {
public:
	void AddPart(const char operation, const double value) {
		parts.emplace_back( operation, value );
	}
	double Apply(double value) const {
		for (auto& part : parts) {
			value = part.Apply(value);
		}
		return value;
	}
	void Invert() {
		for (auto& part : parts) {
			part.Invert();
		}
		reverse(parts.begin(), parts.end());
	}
private:
	vector<FunctionParts> parts;
};

//struct Image {
//	double quality;
//	double freshness;
//	double rating;
//};
//
//struct Params {
//	double a;
//	double b;
//	double c;
//};
//
//Function MakeWeightFunction(const Params& params,
//	const Image& image) {
//	Function function;
//	function.AddPart('*', params.a);
//	function.AddPart('-', image.freshness * params.b);
//	function.AddPart('+', image.rating * params.c);
//	return function;
//}
//
//double ComputeImageWeight(const Params& params, const Image& image) {
//	Function function = MakeWeightFunction(params, image);
//	return function.Apply(image.quality);
//}
//
//double ComputeQualityByWeight(const Params& params,
//	const Image& image,
//	double weight) {
//	Function function = MakeWeightFunction(params, image);
//	function.Invert();
//	return function.Apply(weight);
//}
//
//int main()
//{
//	Image image = { 10, 2, 6 };
//	Params params = { 4, 2, 6 };
//	cout << ComputeImageWeight(params, image) << endl;
//	cout << ComputeQualityByWeight(params, image, 52) << endl;//
//
//#ifdef _MSC_VER
//	system("pause");
//#endif
//
//	return 0;
//}