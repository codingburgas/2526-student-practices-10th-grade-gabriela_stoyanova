#include "User.h"
#include <fstream>
#include <iostream>
#include <conio.h>
#include <limits>

bool User::hasAnyUsers()
{
    std::ifstream in("users.txt");
    return in.good() && !in.eof();
}

bool User::registerUser(const std::string& id, const std::string& pass)
{
    std::ofstream out("users.txt", std::ios::app);
    if (!out)
        return false;
    out << id << " " << pass << "\n";
    return true;
}

bool User::loginUser(const std::string& id, const std::string& pass)
{
    std::ifstream in("users.txt");
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
        std::cout << "2. Book ticket (placeholder)\n";
        std::cout << "3. Logout\n";
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
            std::cout << "Available movies:\n";
            std::cout << "- The Matrix\n- Inception\n- Interstellar\n";
            std::cout << "Press any key to continue...\n";
            _getch();
            break;
        case 2:
            std::cout << "Booking feature not implemented yet.\n";
            std::cout << "Press any key to continue...\n";
            _getch();
            break;
        case 3:
            std::cout << "Logging out...\n";
            break;
        default:
            std::cout << "Invalid choice.\n";
            _getch();
            break;
        }

        system("cls");
    } while (choice != 3);
}
