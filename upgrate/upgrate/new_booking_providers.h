#pragma once

// ����� ����� ��������� ��� ���� � ������������ ���������� ������ Booking � ������������ ��� RAII
#include "booking.h"

#include <stdexcept>
#include <string>
#include <iostream>

using namespace std;

class FlightProvider {
public:
    using BookingId = int;
    using Booking = RAII::Booking<FlightProvider>;
    friend Booking;  // ���� ��������� �������� ������ Booking �������� private-������� ������ ������ FlightProvider

    struct BookingData {
        string city_from;
        string city_to;
        string date;
    };

    Booking Book(const BookingData& data) {
        if (counter >= capacity) {
            throw runtime_error("Flight overbooking");
        }
        ++counter;
        cout << "[Book]: counter = " << counter << endl;
        return { this, counter };
    }

private:
    // �������� ��� ������� � private, ����� � ��� ������� ������ ��������������� friend-����� Booking
    void CancelOrComplete(const Booking& booking) {
        --counter;
        cout << "[CancelOrComplete]: counter = " << counter << endl;
    }

public:
    static int capacity;
    static int counter;
};


class HotelProvider {
public:
    using BookingId = int;
    using Booking = RAII::Booking<HotelProvider>;
    friend Booking;

    struct BookingData {
        string city;
        string date_from;
        string date_to;
    };

    Booking Book(const BookingData& data) {
        if (counter >= capacity) {
            throw runtime_error("Hotel overbooking");
        }
        ++counter;
        return { this, counter };
    }

private:
    void CancelOrComplete(const Booking& booking) {
        --counter;
    }

public:
    static int capacity;
    static int counter;
};
