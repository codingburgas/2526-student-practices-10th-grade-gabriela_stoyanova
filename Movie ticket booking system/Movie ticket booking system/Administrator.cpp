#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <conio.h>
#include "Administrator.h"
#include "Movie.h"
#include "Config.h"
#include <limits>

using namespace std;

void Administrator::changePass(const std::string& currentId)
{
	cout << "Enter new admin user: " << endl;
	cin >> adminId;
	cout << "Enter new password: " << endl;
	cin >> adminPass;

	// If currentId provided, update that admin entry; otherwise append as new admin
	std::vector<std::pair<std::string, std::string>> admins;
	std::ifstream in(Config::ADMINS_FILE);
	if (in)
	{
		std::string id, pass;
		while (in >> id >> pass)
			admins.emplace_back(id, pass);
	}

	bool updated = false;
	if (!currentId.empty())
	{
		for (auto &p : admins)
		{
			if (p.first == currentId)
			{
				p.first = adminId;
				p.second = adminPass;
				updated = true;
				break;
			}
		}
	}

	if (!updated)
		admins.emplace_back(adminId, adminPass);

	std::ofstream out(Config::ADMINS_FILE, std::ios::out | std::ios::trunc);
	if (!out)
	{
		cerr << "Error: could not open admins file for writing." << endl;
		cout << "Press any key to continue..." << endl;
		_getch();
		system("cls");
		return;
	}

	for (auto &p : admins)
		out << p.first << " " << p.second << "\n";

	out.close();
	cout << "Changes saved" << endl;
	cout << "Press any key to continue..." << endl;
	_getch();
	system("cls");
}

bool Administrator::verifyCredentials(const std::string& id, const std::string& pass)
{
	std::ifstream adminData(Config::ADMINS_FILE);
	if (!adminData)
	{
		// If file doesn't exist, deny login
		return false;
	}

	std::string storedId, storedPass;
	// Scan all stored admin entries
	while (adminData >> storedId >> storedPass)
	{
		if (storedId == id && storedPass == pass)
			return true;
	}

	return false;
}

bool Administrator::hasAnyAdmins()
{
	std::ifstream in(Config::ADMINS_FILE);
	return in.good() && !in.eof();
}

bool Administrator::registerAdmin(const std::string& id, const std::string& pass)
{
	std::ofstream out(Config::ADMINS_FILE, std::ios::app);
	if (!out)
		return false;
	out << id << " " << pass << "\n";
	return true;
}

bool Administrator::loginAdmin(const std::string& id, const std::string& pass)
{
	return verifyCredentials(id, pass);
}

void Administrator::adminArea(const std::string& id)
{
	int choice = 0;
	do
	{
		std::cout << "=== Admin Area (" << id << ") ===\n";
		std::cout << "1. List movies\n";
		std::cout << "2. Add movie with time\n";
		std::cout << "3. Change my credentials\n";
		std::cout << "4. Logout\n";
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
			auto movies = MovieManager::listMovies();
			std::cout << "Available movies:\n";
			for (auto &m : movies)
				std::cout << "- " << m << "\n";
			std::cout << "Press any key to continue...\n";
			_getch();
			break;
		}
		case 2:
		{
			std::string title, time;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Movie title: ";
			std::getline(std::cin, title);
			std::cout << "Show time (HH:MM): ";
			std::getline(std::cin, time);
			if (MovieManager::addMovieWithTime(title, time))
				std::cout << "Movie added.\n";
			else
				std::cout << "Failed to add movie.\n";
			std::cout << "Press any key to continue...\n";
			_getch();
			break;
		}
		case 3:
		{
			changePass(id);
			break;
		}
		case 4:
			std::cout << "Logging out...\n";
			break;
		default:
			std::cout << "Invalid choice.\n";
			_getch();
			break;
		}

		system("cls");
	} while (choice != 4);
}
