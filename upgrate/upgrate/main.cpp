#include "test_runner.h"
#include "profile.h"

#include <cstdint>
#include <iterator>
#include <numeric>

#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <vector>
#include <list>
#include <deque>

#include <string_view>
#include <tuple>
#include <memory>


using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
	// Напишите реализацию функции,
	// не копируя сортируемые элементы

	// 1. Если диапазон содержит меньше 2 элементов, выходим из функции
	size_t range_length = range_end - range_begin;
	if (range_length < 2) {
		return;
	}

	// 2. Создаем вектор, содержащий все элементы текущего диапазона
	vector<typename RandomIt::value_type> elements(
			make_move_iterator(range_begin), make_move_iterator(range_end));

	// 3. Разбиваем вектор на три равные части
	auto mid1 = elements.begin() + range_length / 3;
	auto mid2 = mid1 + range_length / 3;

	// 4. Вызываем функцию MergeSort от каждой половины вектора
	MergeSort(elements.begin(), mid1);
	MergeSort(mid1, mid2);
	MergeSort(mid2, elements.end());

	// 5. С помощью алгоритма merge сливаем отсортированные половины
	// в исходный диапазон
	// merge -> http://ru.cppreference.com/w/cpp/algorithm/merge
	vector<typename RandomIt::value_type> temp;
	merge(make_move_iterator(elements.begin()), 
			make_move_iterator(mid1), 
			make_move_iterator(mid1), 
			make_move_iterator(mid2), 
			back_inserter(temp)
	);
	merge(make_move_iterator(temp.begin()), make_move_iterator(temp.end()),
		make_move_iterator(mid2), make_move_iterator(elements.end()), range_begin);
}

void TestIntVector() {
	vector<int> numbers = { 6, 1, 3, 9, 1, 9, 8, 12, 1 };
	MergeSort(begin(numbers), end(numbers));
	ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestIntVector);

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}