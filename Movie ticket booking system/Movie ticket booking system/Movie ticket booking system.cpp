// Movie ticket booking system\Movie ticket booking system.cpp
#include <iostream>
#include <conio.h>
#include <limits>
#include "Administrator.h"
#include "User.h"
#include "Config.h"

int main()
{
    Config::ensureDataDirectory();

    Administrator admin;
    int choice = 0;

    do
    {
        std::cout << "=== Movie Ticket Booking - Main Menu ===\n";
        std::cout << "1. User Menu\n";
        std::cout << "2. Admin Menu\n";
        std::cout << "3. Exit\n";
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
            // User Menu: Login or Register
            int userChoice = 0;
            do
            {
                std::cout << "=== User Menu ===\n";
                std::cout << "1. Login\n";
                std::cout << "2. Register\n";
                std::cout << "3. Back to Main Menu\n";
                std::cout << "Enter choice: ";

                if (!(std::cin >> userChoice))
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    userChoice = -1;
                }

                switch (userChoice)
                {
                case 1:
                {
                    system("cls");
                    User user;
                    std::string id, pass;
                    std::cout << "=== User Login ===\n";
                    std::cout << "User ID: ";
                    std::cin >> id;
                    std::cout << "Password: ";
                    std::cin >> pass;

                    if (user.loginUser(id, pass))
                    {
                        std::cout << "Login successful. Entering user area...\n";
                        std::cout << "Press any key to continue...\n";
                        _getch();
                        system("cls");
                        user.userArea(id);
                    }
                    else
                    {
                        std::cout << "Invalid credentials.\n";
                        std::cout << "Press any key to continue...\n";
                        _getch();
                    }
                    system("cls");
                    break;
                }
                case 2:
                {
                    system("cls");
                    User user;
                    std::string id, pass;
                    std::cout << "=== User Registration ===\n";
                    std::cout << "User ID: ";
                    std::cin >> id;
                    std::cout << "Password: ";
                    std::cin >> pass;

                    if (user.registerUser(id, pass))
                    {
                        std::cout << "Registration successful. You can now login.\n";
                    }
                    else
                    {
                        std::cout << "Registration failed.\n";
                    }
                    std::cout << "Press any key to continue...\n";
                    _getch();
                    system("cls");
                    break;
                }
                case 3:
                {
                    system("cls");
                    break;
                }
                default:
                    std::cout << "Invalid selection. Press any key to try again...\n";
                    _getch();
                    system("cls");
                    break;
                }
            } while (userChoice != 3);
            break;
        }
        case 2:
        {
            system("cls");
            // Admin Menu: Login or Register
            int adminChoice = 0;
            do
            {
                std::cout << "=== Admin Menu ===\n";
                std::cout << "1. Login\n";
                std::cout << "2. Register\n";
                std::cout << "3. Back to Main Menu\n";
                std::cout << "Enter choice: ";

                if (!(std::cin >> adminChoice))
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    adminChoice = -1;
                }

                switch (adminChoice)
                {
                case 1:
                {
                    system("cls");
                    std::string id, pass;
                    std::cout << "=== Admin Login ===\n";
                    std::cout << "Admin ID: ";
                    std::cin >> id;
                    std::cout << "Password: ";
                    std::cin >> pass;

                    if (admin.loginAdmin(id, pass))
                    {
                        std::cout << "Login successful. Entering admin area...\n";
                        std::cout << "Press any key to continue...\n";
                        _getch();
                        system("cls");
                        admin.adminArea(id);
                    }
                    else
                    {
                        std::cout << "Invalid admin credentials.\n";
                        std::cout << "Press any key to continue...\n";
                        _getch();
                    }
                    system("cls");
                    break;
                }
                case 2:
                {
                    system("cls");
                    std::string id, pass;
                    std::cout << "=== Admin Registration ===\n";
                    std::cout << "Admin ID: ";
                    std::cin >> id;
                    std::cout << "Password: ";
                    std::cin >> pass;

                    if (admin.registerAdmin(id, pass))
                    {
                        std::cout << "Admin registration successful. You can now login.\n";
                    }
                    else
                    {
                        std::cout << "Admin registration failed.\n";
                    }
                    std::cout << "Press any key to continue...\n";
                    _getch();
                    system("cls");
                    break;
                }
                case 3:
                {
                    system("cls");
                    break;
                }
                default:
                    std::cout << "Invalid selection. Press any key to try again...\n";
                    _getch();
                    system("cls");
                    break;
                }
            } while (adminChoice != 3);
            break;
        }
        case 3:
            std::cout << "Exiting...\n";
            break;
        default:
            std::cout << "Invalid selection. Press any key to try again...\n";
            _getch();
            break;
        }

        system("cls");
    } while (choice != 3);

    return 0;
}
