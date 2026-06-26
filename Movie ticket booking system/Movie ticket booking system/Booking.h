#pragma once
#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include <vector>

struct BookingInfo {
    std::string userId;
    std::string movieTitle;
    std::string showTime;
    int seatNumber;
};

namespace BookingManager {
    // Old interface for backward compatibility
    std::vector<std::string> getMovieShowTimes(const std::string& movieTitle);
    std::vector<int> getBookedSeats(const std::string& movieTitle, const std::string& showTime);
    bool bookTicket(const std::string& userId, const std::string& movieTitle, const std::string& showTime, int seat);
    std::vector<BookingInfo> getUserBookingsLegacy(const std::string& userId);
    bool cancelBooking(const std::string& userId, const std::string& movieTitle, const std::string& showTime, int seat);
}

#endif // BOOKING_H
