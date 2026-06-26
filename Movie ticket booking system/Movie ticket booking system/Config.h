#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace Config {
    const std::string DATA_DIR = "data/";
    const std::string ADMINS_FILE = DATA_DIR + "admins.txt";
    const std::string USERS_FILE = DATA_DIR + "users.txt";
    const std::string MOVIES_FILE = DATA_DIR + "movies.txt";
    const std::string BOOKINGS_FILE = DATA_DIR + "bookings.txt";
    const std::string CINEMAS_FILE = DATA_DIR + "cinemas.txt";
    const std::string SHOWS_FILE = DATA_DIR + "shows.txt";
    const std::string SEATS_DIR = DATA_DIR + "seats/";
    const std::string PAYMENTS_FILE = DATA_DIR + "payments.txt";
    const std::string NOTIFICATIONS_FILE = DATA_DIR + "notifications.txt";
    const int SEATS_PER_MOVIE = 50;
    const int SEAT_ROWS = 5;
    const int SEATS_PER_ROW = 10;

    bool ensureDataDirectory();
}

#endif // CONFIG_H
