#include <cstdint>
#include <algorithm>

using namespace std;

// ���������� SimpleVector � ���� �����
// � ��������� ��� �� ��������

template <typename T>
class SimpleVector {
public:
	SimpleVector() = default;
	explicit SimpleVector(size_t size)
		: size(size)
		, capacity(size)
		, data(new T[size])
	{}

	//SimpleVector(const SimpleVector& rhs) = delete;

	~SimpleVector() {
		delete[] data;
	}

	//void operator=(const SimpleVector& rhs) {
	//	if (rhs.size <= capacity) {
	//		// � ��� ���������� ������ - ������ �������� ��������
	//		copy(rhs.begin(), rhs.end(), begin());
	//		size = rhs.size;
	//	}
	//	else {
	//		// ��� ��� ���������� ������ copy-and-swap.
	//		// �� ������ ��������� ������ � �������
	//		// ������������ �����������, � ����� ���������� ��� ���� �� ������.
	//		// ��� �� ��������� ���� �����:
	//		//  - �������� ������������ ���� � ������������ �����������
	//		//    � ��������� ������������
	//		//  - ������������ ������������� ��������� ������������ �����������
	//		//    � ��������� ������������
	//		SimpleVector<T> tmp(rhs);
	//		swap(tmp.data, data);
	//		swap(tmp.size, size);
	//		swap(tmp.capacity, capacity);
	//	}
	//}

	T& operator[](size_t index) { return data[index]; }

	T* begin() { return data; }
	T* end() { return &data[size]; }

	const T* begin() const { return data; }
	const T* end() const { return &data[size]; }

	size_t Size() const { return size; }
	size_t Capacity() const { return capacity; }

	//void PushBack(T& value) {
	//	if(size >= capacity) {
	//		auto new_cap = capacity == 0 ? 1 : 2 * capacity;
	//		auto new_data = new T[new_cap];
	//		move(begin(), end(), new_data);
	//		delete[] data;
	//		data = new_data;
	//		capacity = new_cap;
	//	}
	//	data[size++] = move(value);
	//}

	void PushBack(T value) {
		if (size >= capacity) {
			auto new_cap = capacity == 0 ? 1 : 2 * capacity;
			auto new_data = new T[new_cap];
			move(begin(), end(), new_data);
			delete[] data;
			data = new_data;
			capacity = new_cap;
		}
		data[size++] = move(value);
	}

private:
	T* data = nullptr;
	size_t size = 0;
	size_t capacity = 0;
};

