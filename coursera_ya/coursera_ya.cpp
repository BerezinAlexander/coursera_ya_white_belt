#include "test_runner.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename T>
class Table {
public:
	Table(size_t col, size_t row) {
		Resize(col, row);
	}

	vector<T>& operator[](const size_t num) {
		return table[num];
	}

	const vector<T>& operator[](const size_t num) const {
		return table[num];
	}

	void Resize(size_t col, size_t row) {
		table.resize(col);
		for (auto& vec_r : table) {
			vec_r.resize(row);
		}
	}

	pair<size_t, size_t> Size() const {
		return{ table.size(), (table.empty()) ? 0 : table.begin()->size() };
	}

private:
	vector<vector<T>> table;
};

void TestTable() {
	Table<int> t(1, 1);
	ASSERT_EQUAL(t.Size().first, 1u);
	ASSERT_EQUAL(t.Size().second, 1u);
	t[0][0] = 42;
	ASSERT_EQUAL(t[0][0], 42);
	t.Resize(3, 4);
	ASSERT_EQUAL(t.Size().first, 3u);
	ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestTable);

#ifdef _MSC_VER
	system("pause");
#endif
}
