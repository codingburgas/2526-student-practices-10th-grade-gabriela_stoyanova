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
    const int SEATS_PER_MOVIE = 50;

    bool ensureDataDirectory();
}

#endif // CONFIG_H
