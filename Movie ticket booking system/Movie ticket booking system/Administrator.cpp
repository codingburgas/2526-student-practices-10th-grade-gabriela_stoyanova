#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <conio.h>
#include "Administrator.h"
#include "Movie.h"
#include "Config.h"
#include "Cinema.h"
#include "MovieEnhanced.h"
#include "Seat.h"
#include <limits>
#include <map>
#include <algorithm>
#include <sstream>

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

int Administrator::getUserCount()
{
	std::ifstream in(Config::USERS_FILE);
	if (!in)
		return 0;

	int count = 0;
	std::string line;
	while (std::getline(in, line))
	{
		if (!line.empty())
			count++;
	}
	return count;
}

void Administrator::adminArea(const std::string& id)
{
	int choice = 0;
	do
	{
		system("cls");
		std::cout << "=== Admin Area (" << id << ") ===\n";
		std::cout << "1. Manage Cinemas & Halls\n";
		std::cout << "2. Manage Movies & Shows\n";
		std::cout << "3. View Statistics\n";
		std::cout << "4. View Booking Reports\n";
		std::cout << "5. View Revenue Reports\n";
		std::cout << "6. Manage Users\n";
		std::cout << "7. Change my credentials\n";
		std::cout << "8. Logout\n";
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
			manageCinemas();
			break;
		}
		case 2:
		{
			manageMoviesAndShows();
			break;
		}
		case 3:
		{
			system("cls");
			int userCount = getUserCount();
			std::cout << "=== System Statistics ===\n";
			std::cout << "Total registered users: " << userCount << "\n";

			auto cinemas = CinemaManager::getAllCinemas();
			std::cout << "Total cinemas: " << cinemas.size() << "\n";

			int totalHalls = 0;
			for (const auto& cinema : cinemas) {
				totalHalls += cinema.getHalls().size();
			}
			std::cout << "Total halls: " << totalHalls << "\n";

			auto movies = MovieManager::getAllMovies();
			std::cout << "Total movies: " << movies.size() << "\n";

			int totalShows = 0;
			for (const auto& movie : movies) {
				totalShows += MovieManager::getMovieShows(movie.getMovieId()).size();
			}
			std::cout << "Total shows: " << totalShows << "\n";

			// Count total bookings
			int totalBookings = 0;
			std::ifstream bookingFile(Config::BOOKINGS_FILE);
			if (bookingFile) {
				std::string line;
				while (std::getline(bookingFile, line)) {
					if (!line.empty()) {
						totalBookings++;
					}
				}
				bookingFile.close();
			}
			std::cout << "Total bookings: " << totalBookings << "\n";

			std::cout << "\nPress any key to continue...\n";
			_getch();
			break;
		}
		case 4:
		{
			viewBookingReports();
			break;
		}
		case 5:
		{
			viewRevenueReports();
			break;
		}
		case 6:
		{
			manageUsers();
			break;
		}
		case 7:
		{
			changePass(id);
			break;
		}
		case 8:
			std::cout << "Logging out...\n";
			break;
		default:
			std::cout << "Invalid choice.\n";
			_getch();
			break;
		}

	} while (choice != 8);
}

void Administrator::manageCinemas()
{
	int choice = 0;
	do
	{
		system("cls");
		std::cout << "=== Cinema Management ===\n";
		std::cout << "1. Add Cinema\n";
		std::cout << "2. View All Cinemas\n";
		std::cout << "3. Back to Admin Menu\n";
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
			int cinemaId;
			std::string cinemaName, city, address;
			int hallCount;

			std::cout << "=== Add New Cinema ===\n";
			std::cout << "Cinema ID: ";
			std::cin >> cinemaId;
			std::cin.ignore();

			std::cout << "Cinema Name: ";
			std::getline(std::cin, cinemaName);

			std::cout << "City: ";
			std::getline(std::cin, city);

			std::cout << "Address: ";
			std::getline(std::cin, address);

			std::cout << "Number of Halls: ";
			std::cin >> hallCount;
			std::cin.ignore();

			Cinema cinema(cinemaId, cinemaName, city, address);

			for (int i = 0; i < hallCount; i++)
			{
				int hallId, seats;
				std::string hallName;

				std::cout << "\nHall " << (i + 1) << ":\n";
				std::cout << "Hall ID: ";
				std::cin >> hallId;
				std::cin.ignore();

				std::cout << "Hall Name: ";
				std::getline(std::cin, hallName);

				std::cout << "Number of Seats: ";
				std::cin >> seats;
				std::cin.ignore();

				cinema.addHall(std::make_shared<CinemaHall>(hallId, hallName, seats));
			}

			if (CinemaManager::addCinema(cinema))
			{
				std::cout << "\nCinema added successfully!\n";
			}
			else
			{
				std::cout << "\nFailed to add cinema.\n";
			}

			std::cout << "Press any key to continue...\n";
			_getch();
			break;
		}
		case 2:
		{
			system("cls");
			auto cinemas = CinemaManager::getAllCinemas();
			std::cout << "=== All Cinemas ===\n";

			if (cinemas.empty())
			{
				std::cout << "No cinemas found.\n";
			}
			else
			{
				for (const auto& cinema : cinemas)
				{
					std::cout << "\nCinema ID: " << cinema.getCinemaId() << "\n";
					std::cout << "Name: " << cinema.getCinemaName() << "\n";
					std::cout << "City: " << cinema.getCity() << "\n";
					std::cout << "Address: " << cinema.getAddress() << "\n";
					std::cout << "Halls: " << cinema.getHalls().size() << "\n";

					for (const auto& hall : cinema.getHalls())
					{
						std::cout << "  - " << hall->getHallName() << " (Seats: " << hall->getTotalSeats() << ")\n";
					}
				}
			}

			std::cout << "Press any key to continue...\n";
			_getch();
			break;
		}
		case 3:
			break;
		default:
			std::cout << "Invalid choice.\n";
			_getch();
			break;
		}

	} while (choice != 3);
}

void Administrator::manageMoviesAndShows()
{
	int choice = 0;
	do
	{
		system("cls");
		std::cout << "=== Movie & Show Management ===\n";
		std::cout << "1. Add Movie\n";
		std::cout << "2. Add Show\n";
		std::cout << "3. View All Movies\n";
		std::cout << "4. View All Shows\n";
		std::cout << "5. Back to Admin Menu\n";
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
			int movieId, duration;
			std::string title, language, genre, releaseDate, director;

			std::cout << "=== Add New Movie ===\n";
			std::cout << "Movie ID: ";
			std::cin >> movieId;
			std::cin.ignore();

			std::cout << "Title: ";
			std::getline(std::cin, title);

			std::cout << "Language: ";
			std::getline(std::cin, language);

			std::cout << "Genre: ";
			std::getline(std::cin, genre);

			std::cout << "Release Date (YYYY-MM-DD): ";
			std::getline(std::cin, releaseDate);

			std::cout << "Director: ";
			std::getline(std::cin, director);

			std::cout << "Duration (minutes): ";
			std::cin >> duration;
			std::cin.ignore();

			Movie movie(movieId, title, language, genre, releaseDate);
			movie.setDirector(director);
			movie.setDuration(duration);

			if (MovieManager::addMovie(movie))
			{
				std::cout << "\nMovie added successfully!\n";
			}
			else
			{
				std::cout << "\nFailed to add movie.\n";
			}

			std::cout << "Press any key to continue...\n";
			_getch();
			break;
		}
		case 2:
		{
			system("cls");
			int showId, movieId, cinemaId, hallId;
			std::string showTime;
			double silverPrice, goldPrice, platinumPrice;

			std::cout << "=== Add New Show ===\n";
			std::cout << "Show ID: ";
			std::cin >> showId;

			std::cout << "Movie ID: ";
			std::cin >> movieId;

			std::cout << "Cinema ID: ";
			std::cin >> cinemaId;

			std::cout << "Hall ID: ";
			std::cin >> hallId;

			std::cin.ignore();
			std::cout << "Show Time (HH:MM): ";
			std::getline(std::cin, showTime);

			std::cout << "Silver Price: ";
			std::cin >> silverPrice;

			std::cout << "Gold Price: ";
			std::cin >> goldPrice;

			std::cout << "Platinum Price: ";
			std::cin >> platinumPrice;

			Show show{
				showId,
				movieId,
				cinemaId,
				hallId,
				showTime,
				silverPrice,
				goldPrice,
				platinumPrice,
				true
			};

			if (MovieManager::addShow(show))
			{
				// Initialize seat layout for this show
				SeatManager::initializeShowSeats(hallId, showId, Config::SEAT_ROWS, Config::SEATS_PER_ROW);
				std::cout << "\nShow added successfully!\n";
			}
			else
			{
				std::cout << "\nFailed to add show.\n";
			}

			std::cout << "Press any key to continue...\n";
			_getch();
			break;
		}
		case 3:
		{
			system("cls");
			auto movies = MovieManager::getAllMovies();
			std::cout << "=== All Movies ===\n";

			if (movies.empty())
			{
				std::cout << "No movies found.\n";
			}
			else
			{
				for (const auto& movie : movies)
				{
					std::cout << "\nID: " << movie.getMovieId() << "\n";
					std::cout << "Title: " << movie.getTitle() << "\n";
					std::cout << "Language: " << movie.getLanguage() << "\n";
					std::cout << "Genre: " << movie.getGenre() << "\n";
					std::cout << "Release Date: " << movie.getReleaseDate() << "\n";
					std::cout << "Director: " << movie.getDirector() << "\n";
					std::cout << "Duration: " << movie.getDurationMinutes() << " minutes\n";
				}
			}

			std::cout << "Press any key to continue...\n";
			_getch();
			break;
		}
		case 4:
		{
			system("cls");
			auto movies = MovieManager::getAllMovies();
			std::cout << "=== All Shows ===\n";

			if (movies.empty())
			{
				std::cout << "No movies found.\n";
			}
			else
			{
				for (const auto& movie : movies)
				{
					auto shows = MovieManager::getMovieShows(movie.getMovieId());
					if (!shows.empty())
					{
						std::cout << "\nMovie: " << movie.getTitle() << "\n";
						for (const auto& show : shows)
						{
							std::cout << "  Show ID: " << show.showId << "\n";
							std::cout << "    Cinema ID: " << show.cinemaId << "\n";
							std::cout << "    Hall ID: " << show.hallId << "\n";
							std::cout << "    Time: " << show.showTime << "\n";
							std::cout << "    Silver: " << show.silverPrice << "\n";
							std::cout << "    Gold: " << show.goldPrice << "\n";
							std::cout << "    Platinum: " << show.platinumPrice << "\n";
						}
					}
				}
			}

			std::cout << "Press any key to continue...\n";
			_getch();
			break;
		}
		case 5:
			break;
		default:
			std::cout << "Invalid choice.\n";
			_getch();
			break;
		}

	} while (choice != 5);
}

void Administrator::viewBookingReports()
{
	system("cls");
	std::cout << "=== Booking Reports ===\n\n";

	std::map<std::string, int> movieBookings;
	std::map<std::string, int> userBookings;

	std::ifstream bookingFile(Config::BOOKINGS_FILE);
	if (!bookingFile) {
		std::cout << "No booking data available.\n";
		std::cout << "Press any key to continue...\n";
		_getch();
		return;
	}

	std::string line;
	int totalBookings = 0;
	while (std::getline(bookingFile, line)) {
		if (line.empty()) continue;

		std::stringstream ss(line);
		std::string userId, movieTitle, showTime;
		int seatNumber;

		if (std::getline(ss, userId, '|') && std::getline(ss, movieTitle, '|') &&
			std::getline(ss, showTime, '|') && (ss >> seatNumber)) {
			movieBookings[movieTitle]++;
			userBookings[userId]++;
			totalBookings++;
		}
	}
	bookingFile.close();

	std::cout << "Total Bookings: " << totalBookings << "\n\n";

	std::cout << "=== Bookings by Movie ===\n";
	for (const auto& pair : movieBookings) {
		std::cout << pair.first << ": " << pair.second << " bookings\n";
	}

	std::cout << "\n=== Top 5 Users ===\n";
	std::vector<std::pair<std::string, int>> userList(userBookings.begin(), userBookings.end());
	std::sort(userList.begin(), userList.end(),
		[](const auto& a, const auto& b) { return a.second > b.second; });

	int count = 0;
	for (const auto& pair : userList) {
		if (count++ >= 5) break;
		std::cout << pair.first << ": " << pair.second << " bookings\n";
	}

	std::cout << "\nPress any key to continue...\n";
	_getch();
}

void Administrator::viewRevenueReports()
{
	system("cls");
	std::cout << "=== Revenue Reports ===\n\n";

	double totalRevenue = 0.0;
	std::map<std::string, double> movieRevenue;

	std::ifstream bookingFile(Config::BOOKINGS_FILE);
	if (!bookingFile) {
		std::cout << "No booking data available.\n";
		std::cout << "Press any key to continue...\n";
		_getch();
		return;
	}

	std::string line;
	int totalBookings = 0;
	while (std::getline(bookingFile, line)) {
		if (line.empty()) continue;

		std::stringstream ss(line);
		std::string userId, movieTitle, showTime;
		int seatNumber;

		if (std::getline(ss, userId, '|') && std::getline(ss, movieTitle, '|') &&
			std::getline(ss, showTime, '|') && (ss >> seatNumber)) {
			// Average seat price: 200 (Gold seats)
			double price = 200.0;
			movieRevenue[movieTitle] += price;
			totalRevenue += price;
			totalBookings++;
		}
	}
	bookingFile.close();

	std::cout << "Total Revenue: Rs. " << totalRevenue << "\n";
	std::cout << "Total Bookings: " << totalBookings << "\n";
	std::cout << "Average Revenue per Booking: Rs. " << (totalBookings > 0 ? totalRevenue / totalBookings : 0) << "\n\n";

	std::cout << "=== Revenue by Movie ===\n";
	for (const auto& pair : movieRevenue) {
		std::cout << pair.first << ": Rs. " << pair.second << "\n";
	}

	std::cout << "\nPress any key to continue...\n";
	_getch();
}

void Administrator::manageUsers()
{
	int choice = 0;
	do
	{
		system("cls");
		std::cout << "=== User Management ===\n";
		std::cout << "1. View All Users\n";
		std::cout << "2. Back to Admin Menu\n";
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
			std::cout << "=== All Registered Users ===\n\n";

			std::vector<std::pair<std::string, std::string>> users;
			std::ifstream userFile(Config::USERS_FILE);
			if (!userFile) {
				std::cout << "No users registered yet.\n";
			}
			else {
				std::string id, pass;
				int userCount = 0;
				while (userFile >> id >> pass) {
					users.push_back({id, pass});
					std::cout << (++userCount) << ". User ID: " << id << "\n";
				}
				userFile.close();

				if (users.empty()) {
					std::cout << "No users found.\n";
				}
				else {
					std::cout << "\nTotal users: " << users.size() << "\n";
				}
			}

			std::cout << "\nPress any key to continue...\n";
			_getch();
			break;
		}
		case 2:
			break;
		default:
			std::cout << "Invalid choice.\n";
			_getch();
			break;
		}

	} while (choice != 2);
}