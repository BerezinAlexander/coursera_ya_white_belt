#include "test_runner.h"

#include <cstddef>  // нужно для nullptr_t

using namespace std;

// Реализуйте шаблон класса UniquePtr
#include <cstddef>
#include <utility>

template <typename T>
class UniquePtr {
private:
    T* ptr_;

public:
    UniquePtr() : ptr_(nullptr) {}
    UniquePtr(T * ptr) : ptr_(ptr) {}
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr(UniquePtr&& other) : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }
    UniquePtr& operator = (const UniquePtr&) = delete;
    UniquePtr& operator = (std::nullptr_t) {
        Reset(nullptr);
        return *this;
    }
    UniquePtr& operator = (UniquePtr&& other) {
        Reset(other.ptr_);
        other.ptr_ = nullptr;
        return *this;
    }
    ~UniquePtr() {
        delete ptr_;
    }
    T& operator * () const {
        return *ptr_;
    }
    T * operator -> () const {
        return ptr_;
    }
    T * Release() {
        auto result = ptr_;
        ptr_ = nullptr;
        return result;
    }
    void Reset(T * ptr) {
        delete ptr_;
        ptr_ = ptr;
    }
    void Swap(UniquePtr& other) {
        std::swap(ptr_, other.ptr_);
    }
    T * Get() const {
        return ptr_;
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

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}