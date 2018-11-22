#include <iostream>
#include <string>
#include <vector>
#include <exception>

using namespace std;

template <typename T>
class Deque {
public:
	Deque() = default;

	bool Empty() const {
		return vFront.empty() && vBack.empty();
	}

	size_t Size() const {
		return (Empty()) ? 0 : (vFront.size() + vBack.size());
	}

	T& operator[](size_t index) {
		return At(index);
	}

	const T& operator[](size_t index) const {
		return At(index);
	}

	T& At(size_t index) {
		if (index > (Size() - 1))
			throw out_of_range("");

		size_t pos = index;
		if (pos >= vFront.size()) {
			pos -= vFront.size();
			return vBack.at(pos);
		}
		else {
			return vFront.at(vFront.size() - 1 - pos);//   *(next(vFront.rbegin(), pos));
		}
	}

	const T& At(size_t index) const {
		if (index >= Size())
			throw out_of_range("");

		size_t pos = index;
		if (pos >= vFront.size()) {
			pos -= vFront.size();
			return vBack.at(pos);
		}
		else {
			return vFront.at(vFront.size() - 1 - pos);//   *(next(vFront.rbegin(), pos));
		}
	}

	T& Front() {
		if (Empty())
			throw out_of_range("");
		else if (!vFront.empty())
			return vFront.back();
		else
			return vBack.front();
	}


	const T& Front() const {
		if(Empty())
			throw out_of_range("");
		else if (!vFront.empty()) 
			return vFront.back();
		else 
			return vBack.front();
	}

	T& Back() {
		if (Empty())
			throw out_of_range("");
		else if (!vBack.empty())
			return vBack.back();
		else
			return vFront.front();
	}

	const T& Back() const {
		if (Empty())
			throw out_of_range("");
		else if (!vBack.empty())
			return vBack.back();
		else
			return vFront.front();
	}

	void PushBack(const T& val) {
		vBack.emplace_back(val);
	}

	void PushFront(const T& val) {
		vFront.emplace_back(val);
	}

private:
	vector<T> vFront;
	vector<T> vBack;
};

int main() {
	//vector<int> vi;
	//cout << vi.size() << endl;


	//d.PushBack(4);
	//d.PushFront(2);
	//d.PushFront(1);

	//int i = d.At(3);
	//d.At(3) = 5;
	//d[2] = 6;
	//d[1] = 7;
	//const int v1 = d[2];
	//const int v2 = d.At(2);
	//int v3 = d.Front();
	//const int v4 = d.Front();

	//cout << d.At(1) << endl;

	//for (int i : {0, 1, 2, 3}) {
	//	cout << d.At(i) << " ";
	//}
	//cout << endl;

	//cout << d.Back() << endl;
	//cout << d.Front() << endl;
	


#ifdef _MSC_VER
	system("pause");
#endif
}
