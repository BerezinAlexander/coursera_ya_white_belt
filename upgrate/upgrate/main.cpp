#include "test_runner.h"

//#include <memory>

#include <cstddef>  // нужно для nullptr_t

using namespace std;

// Реализуйте шаблон класса UniquePtr
template <typename T>
class UniquePtr {
private:
    T* value;
public:
    UniquePtr() : value(nullptr) {}
    UniquePtr(T * ptr) : value(ptr) {}

    UniquePtr(UniquePtr&& other) {
        value = other.Release();
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator = (const UniquePtr&) = delete;
    
    UniquePtr& operator = (nullptr_t) { 
        if (value) 
            delete value; 
        value = nullptr;
        return (*this);
    }
    UniquePtr& operator = (UniquePtr&& other) {
        value = other.Release();
        return *this;
    }

    ~UniquePtr() {
        if (value)
            delete value;
        value = nullptr;
    }

    T& operator * () const {
        return *value;
    }

    T * operator -> () const {
        return value;
    }

    T * Release() {
        T* result = value;
        value = nullptr;
        return result;
    }

    void Reset(T * ptr) {
        if (value)
            delete value;
        value = ptr;
    }

    void Swap(UniquePtr& other) {
        T* tmp = value;
        value = other.Release();
        other.Reset(tmp);
    }

    T * Get() const {
        return value;
    }
};


struct Item {
    static int counter;
    int value;
    Item(int v = 0) : value(v) {
        ++counter;
    }
    Item(const Item& other) : value(other.value) {
        ++counter;
    }
    ~Item() {
        --counter;
    }
};

int Item::counter = 0;


void TestLifetime() {
    Item::counter = 0;
    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        ptr.Reset(new Item);
        ASSERT_EQUAL(Item::counter, 1);
    }
    ASSERT_EQUAL(Item::counter, 0);

    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        auto rawPtr = ptr.Release();
        ASSERT_EQUAL(Item::counter, 1);

        delete rawPtr;
        ASSERT_EQUAL(Item::counter, 0);
    }
    ASSERT_EQUAL(Item::counter, 0);
}

void TestGetters() {
    UniquePtr<Item> ptr(new Item(42));
    ASSERT_EQUAL(ptr.Get()->value, 42);
    ASSERT_EQUAL((*ptr).value, 42);
    ASSERT_EQUAL(ptr->value, 42);
}

void TestSwap() {
    UniquePtr<Item> ptr(new Item(42));
    UniquePtr<Item> ptr2(new Item(38));
    ptr.Swap(ptr2);

    ASSERT_EQUAL(ptr.Get()->value, 38);
    ASSERT_EQUAL((*ptr).value, 38);
    ASSERT_EQUAL(ptr->value, 38);

    ASSERT_EQUAL(ptr2.Get()->value, 42);
    ASSERT_EQUAL((*ptr2).value, 42);
    ASSERT_EQUAL(ptr2->value, 42);
}

void TestReset() {
    UniquePtr<Item> ptr(new Item(42));
    Item* newItem = new Item(38);
    ptr.Reset(newItem);

    ASSERT_EQUAL(ptr.Get()->value, 38);
    ASSERT_EQUAL((*ptr).value, 38);
    ASSERT_EQUAL(ptr->value, 38);
}

void TestFree() {
    {
        UniquePtr<Item> ptr(new Item(42));
    }

    {
        UniquePtr<Item> ptr(new Item(42));
        Item* newItem = new Item(38);
        ptr.Reset(newItem);
        ptr.Reset(new Item(25));
        Item* item = ptr.Release();
        ptr.Reset(new Item(15));
        UniquePtr<Item> ptr2(item);
        ptr = move(ptr2);
        UniquePtr<Item> ptr3(move(ptr));
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLifetime);
    RUN_TEST(tr, TestGetters);
    RUN_TEST(tr, TestSwap);
    RUN_TEST(tr, TestReset);
    RUN_TEST(tr, TestFree);

   // unique_ptr<int> ptr;

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}