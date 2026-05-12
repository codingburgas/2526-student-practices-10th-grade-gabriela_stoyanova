#include "Booking.h"
#include "Config.h"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace BookingManager {

std::vector<std::string> getMovieShowTimes(const std::string& movieTitle)
{
    std::vector<std::string> times;
    std::ifstream in(Config::MOVIES_FILE);
    if (!in) return times;

    std::string line;
    while (std::getline(in, line))
    {
        if (line.find(movieTitle) == 0)
        {
            size_t pos = line.find('|');
            if (pos != std::string::npos)
            {
                std::string time = line.substr(pos + 1);
                if (std::find(times.begin(), times.end(), time) == times.end())
                    times.push_back(time);
            }
        }
    }
    return times;
}

std::vector<int> getBookedSeats(const std::string& movieTitle, const std::string& showTime)
{
    std::vector<int> booked;
    std::ifstream in(Config::BOOKINGS_FILE);
    if (!in) return booked;

    std::string line;
    while (std::getline(in, line))
    {
        std::stringstream ss(line);
        std::string userId, title, time;
        int seat;
        if (ss >> userId && ss.ignore(1) && std::getline(ss, title, '|') && ss >> time && ss >> seat)
        {
            if (title == movieTitle && time == showTime)
                booked.push_back(seat);
        }
    }
    std::sort(booked.begin(), booked.end());
    return booked;
}

bool bookTicket(const std::string& userId, const std::string& movieTitle, const std::string& showTime, int seat)
{
    if (seat < 1 || seat > Config::SEATS_PER_MOVIE) return false;

    auto booked = getBookedSeats(movieTitle, showTime);
    if (std::find(booked.begin(), booked.end(), seat) != booked.end())
        return false; // Seat already booked

    std::ofstream out(Config::BOOKINGS_FILE, std::ios::app);
    if (!out) return false;

    out << userId << "|" << movieTitle << "|" << showTime << "|" << seat << "\n";
    return true;
}

std::vector<BookingInfo> getUserBookings(const std::string& userId)
{
    std::vector<BookingInfo> bookings;
    std::ifstream in(Config::BOOKINGS_FILE);
    if (!in) return bookings;

    std::string line;
    while (std::getline(in, line))
    {
        std::stringstream ss(line);
        std::string user, title, time;
        int seat;
        if (ss >> user && ss.ignore(1) && std::getline(ss, title, '|') && ss >> time && ss >> seat)
        {
            if (user == userId)
                bookings.push_back({user, title, time, seat});
        }
    }
    return bookings;
}

bool cancelBooking(const std::string& userId, const std::string& movieTitle, const std::string& showTime, int seat)
{
    std::vector<std::string> lines;
    std::ifstream in(Config::BOOKINGS_FILE);
    if (!in) return false;

    std::string line;
    bool found = false;
    while (std::getline(in, line))
    {
        std::stringstream ss(line);
        std::string user, title, time;
        int s;
        bool match = false;
        if (ss >> user && ss.ignore(1) && std::getline(ss, title, '|') && ss >> time && ss >> s)
        {
            if (user == userId && title == movieTitle && time == showTime && s == seat)
            {
                found = true;
                match = true;
            }
        }
        if (!match) lines.push_back(line);
    }
    in.close();

    if (found)
    {
        std::ofstream out(Config::BOOKINGS_FILE, std::ios::trunc);
        for (auto &l : lines)
            out << l << "\n";
        out.close();
    }
    return found;
}

}
