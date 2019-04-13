#include <cstdint>
#include <algorithm>

using namespace std;

// Реализуйте SimpleVector в этом файле
// и отправьте его на проверку

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
	//		// У нас достаточно памяти - просто копируем элементы
	//		copy(rhs.begin(), rhs.end(), begin());
	//		size = rhs.size;
	//	}
	//	else {
	//		// Это так называемая идиома copy-and-swap.
	//		// Мы создаём временный вектор с помощью
	//		// конструктора копирования, а затем обмениваем его поля со своими.
	//		// Так мы достигаем двух целей:
	//		//  - избегаем дублирования кода в конструкторе копирования
	//		//    и операторе присваивания
	//		//  - обеспечиваем согласованное поведение конструктора копирования
	//		//    и оператора присваивания
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

