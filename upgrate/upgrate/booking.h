#pragma once

#include <mutex>

using namespace std;

namespace RAII {

    template <class Provider>
    class Booking
    {
    public:
        Booking(Provider * prov, int)
            : p(move(prov))
        {
        }
        Booking() = delete;
        Booking(const Booking & other) = delete;
        Booking(Booking && other)
            : p(move(other.p))
        {
            lock_guard l(m);
            other.p = nullptr;
        }
        Booking& operator=(const Booking& other) = delete;
        Booking& operator=(Booking && other)
        {
            lock_guard l(m);
            if (p != nullptr)
                p->CancelOrComplete(*this);
            p = (move(other.p));
            other.p = nullptr;
        }

        ~Booking()
        {
            if (p != nullptr)
            {
                //      lock_guard l(m);
                p->CancelOrComplete(*this);
                //      p.release();
            }
        }

    private:
        Provider * p;
        mutex m;
    };

}