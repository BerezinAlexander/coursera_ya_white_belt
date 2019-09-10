#pragma once

#include <mutex>

using namespace std;

namespace RAII {

    template <class Provider>
    class Booking
    {
    public:
        Booking(Provider * prov, int) : p(prov) {}
        
        Booking() = delete;
        Booking(const Booking & other) = delete;
        Booking& operator=(const Booking& other) = delete;
        
        Booking(Booking && other)
            : p(other.p)
        {
            other.p = nullptr;
        }

        Booking& operator=(Booking && other)
        {
            p = other.p;
            other.p = nullptr;
            return *this;
        }

        ~Booking()
        {
            if (p) {
                p->CancelOrComplete(*this);
            }
        }

    private:
        Provider * p = nullptr;
    };

}