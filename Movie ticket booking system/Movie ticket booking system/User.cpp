#include "User.h"
#include "Config.h"
#include "Booking.h"
#include "Movie.h"
#include <fstream>
#include <iostream>
#include <conio.h>
#include <limits>

bool User::hasAnyUsers()
{
    std::ifstream in(Config::USERS_FILE);
    return in.good() && !in.eof();
}

bool User::registerUser(const std::string& id, const std::string& pass)
{
    std::ofstream out(Config::USERS_FILE, std::ios::app);
    if (!out)
        return false;
    out << id << " " << pass << "\n";
    return true;
}

bool User::loginUser(const std::string& id, const std::string& pass)
{
    std::ifstream in(Config::USERS_FILE);
    if (!in)
        return false;
    std::string storedId, storedPass;
    while (in >> storedId >> storedPass)
    {
        if (storedId == id && storedPass == pass)
            return true;
    }
    return false;
}

void User::userArea(const std::string& id)
{
    int choice = 0;
    do
    {
        std::cout << "=== User Area (" << id << ") ===\n";
        std::cout << "1. List movies\n";
        std::cout << "2. Book ticket\n";
        std::cout << "3. View my bookings\n";
        std::cout << "4. Cancel booking\n";
        std::cout << "5. Logout\n";
        std::cout << "Enter choice: ";
        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
        }

        switch (choice)
        {
        case 1:
        {
            system("cls");
            auto movies = MovieManager::listMovies();
            std::cout << "Available movies:\n";
            if (movies.empty())
                std::cout << "No movies available.\n";
            else
                for (int i = 0; i < movies.size(); i++)
                    std::cout << (i+1) << ". " << movies[i] << "\n";
            std::cout << "Press any key to continue...\n";
            _getch();
            break;
        }
        case 2:
        {
            system("cls");
            auto movies = MovieManager::listMovies();
            if (movies.empty())
            {
                std::cout << "No movies available to book.\n";
            }
            else
            {
                std::cout << "Available movies:\n";
                for (int i = 0; i < movies.size(); i++)
                    std::cout << (i+1) << ". " << movies[i] << "\n";
                std::cout << "Select movie (or 0 to cancel): ";
                int movieIdx;
                std::cin >> movieIdx;
                if (movieIdx > 0 && movieIdx <= movies.size())
                {
                    std::string movieTitle = movies[movieIdx - 1];
                    auto times = BookingManager::getMovieShowTimes(movieTitle);
                    if (times.empty())
                    {
                        std::cout << "No showtimes available for this movie.\n";
                    }
                    else
                    {
                        std::cout << "Available times:\n";
                        for (int i = 0; i < times.size(); i++)
                            std::cout << (i+1) << ". " << times[i] << "\n";
                        std::cout << "Select time: ";
                        int timeIdx;
                        std::cin >> timeIdx;
                        if (timeIdx > 0 && timeIdx <= times.size())
                        {
                            std::string showTime = times[timeIdx - 1];
                            auto booked = BookingManager::getBookedSeats(movieTitle, showTime);
                            std::cout << "Booked seats: ";
                            for (int s : booked) std::cout << s << " ";
                            std::cout << "\nAvailable seats: 1-" << Config::SEATS_PER_MOVIE << "\n";
                            std::cout << "Enter seat number: ";
                            int seat;
                            std::cin >> seat;
                            if (BookingManager::bookTicket(id, movieTitle, showTime, seat))
                                std::cout << "Ticket booked successfully!\n";
                            else
                                std::cout << "Booking failed (seat taken or invalid).\n";
                        }
                    }
                }
            }
            std::cout << "Press any key to continue...\n";
            _getch();
            break;
        }
        case 3:
        {
            system("cls");
            auto bookings = BookingManager::getUserBookings(id);
            std::cout << "Your bookings:\n";
            if (bookings.empty())
                std::cout << "No bookings.\n";
            else
                for (auto &b : bookings)
                    std::cout << b.movieTitle << " | " << b.showTime << " | Seat " << b.seatNumber << "\n";
            std::cout << "Press any key to continue...\n";
            _getch();
            break;
        }
        case 4:
        {
            system("cls");
            auto bookings = BookingManager::getUserBookings(id);
            if (bookings.empty())
            {
                std::cout << "No bookings to cancel.\n";
            }
            else
            {
                std::cout << "Your bookings:\n";
                for (int i = 0; i < bookings.size(); i++)
                    std::cout << (i+1) << ". " << bookings[i].movieTitle << " | " << bookings[i].showTime << " | Seat " << bookings[i].seatNumber << "\n";
                std::cout << "Select booking to cancel (or 0 to cancel): ";
                int idx;
                std::cin >> idx;
                if (idx > 0 && idx <= bookings.size())
                {
                    auto &b = bookings[idx - 1];
                    if (BookingManager::cancelBooking(id, b.movieTitle, b.showTime, b.seatNumber))
                        std::cout << "Booking cancelled.\n";
                    else
                        std::cout << "Failed to cancel.\n";
                }
            }
            std::cout << "Press any key to continue...\n";
            _getch();
            break;
        }
        case 5:
            std::cout << "Logging out...\n";
            break;
        default:
            std::cout << "Invalid choice.\n";
            _getch();
            break;
        }

        system("cls");
    } while (choice != 5);
}
