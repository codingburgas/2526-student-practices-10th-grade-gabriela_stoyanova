// Movie ticket booking system\Movie ticket booking system.cpp
#include <iostream>
#include <conio.h>
#include <limits>
#include "Administrator.h"
#include "User.h"

int main()
{
    Administrator admin;
    int choice = 0;

    do
    {
        std::cout << "=== Movie Ticket Booking - Main Menu ===\n";
        std::cout << "1. User login\n";
        std::cout << "2. Admin login\n";
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
            // User path: login or register
            {
                User user;
                if (!user.hasAnyUsers())
                {
                    std::string id, pass;
                    std::cout << "No users found. Register a new user.\n";
                    std::cout << "User ID: ";
                    std::cin >> id;
                    std::cout << "Password: ";
                    std::cin >> pass;
                    if (user.registerUser(id, pass))
                        std::cout << "Registration successful. You can now login.\n";
                    else
                        std::cout << "Registration failed.\n";
                    std::cout << "Press any key to continue...\n";
                    _getch();
                }
                else
                {
                    std::string id, pass;
                    std::cout << "User login\n";
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
                }
            }
            break;
        }
        case 2:
        {
            system("cls");
            // Admin path: register or login
            {
                if (!admin.hasAnyAdmins())
                {
                    std::string id, pass;
                    std::cout << "No admins found. Register a new admin.\n";
                    std::cout << "Admin ID: ";
                    std::cin >> id;
                    std::cout << "Password: ";
                    std::cin >> pass;
                    if (admin.registerAdmin(id, pass))
                        std::cout << "Admin registration successful.\n";
                    else
                        std::cout << "Admin registration failed.\n";
                    std::cout << "Press any key to continue...\n";
                    _getch();
                }
                else
                {
                    std::string id, pass;
                    std::cout << "Admin login\n";
                    std::cout << "Admin ID: ";
                    std::cin >> id;
                    std::cout << "Password: ";
                    std::cin >> pass;
                    if (admin.loginAdmin(id, pass))
                    {
                        std::cout << "Login successful. You can change admin credentials now.\n";
                        std::cout << "Press any key to continue...\n";
                        _getch();
                        system("cls");
                        admin.changePass(id);
                    }
                    else
                    {
                        std::cout << "Invalid admin credentials.\n";
                        std::cout << "Press any key to continue...\n";
                        _getch();
                    }
                }
            }

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
