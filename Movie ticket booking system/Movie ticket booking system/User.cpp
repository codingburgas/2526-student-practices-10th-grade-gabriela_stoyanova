#include "User.h"
#include "Config.h"
#include "Booking.h"
#include "MovieEnhanced.h"
#include "Seat.h"
#include "Cinema.h"
#include "Notification.h"
#include <fstream>
#include <iostream>
#include <conio.h>
#include <limits>
#include <sstream>

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

bool User::addPaymentMethod(const std::string& cardNumber, const std::string& cardholderName)
{
	// Store payment method for user
	this->cardNumber = cardNumber;
	return true;
}

bool User::updateProfile(const std::string& email, const std::string& phone)
{
	this->email = email;
	this->phone = phone;
	return true;
}

void User::viewShowsForMovie(int movieId)
{
	system("cls");
	try {
		Movie movie = MovieManager::getMovieById(movieId);
		auto shows = MovieManager::getMovieShows(movieId);

		std::cout << "=== Shows for: " << movie.getTitle() << " ===\n\n";

		if (shows.empty()) {
			std::cout << "No shows available for this movie.\n";
		}
		else {
			std::cout << "Available Shows:\n";
			for (size_t i = 0; i < shows.size(); i++) {
				std::cout << (i + 1) << ". Time: " << shows[i].showTime 
						  << " | Silver: " << shows[i].silverPrice 
						  << " | Gold: " << shows[i].goldPrice 
						  << " | Platinum: " << shows[i].platinumPrice << "\n";
			}
		}
	}
	catch (const std::exception& e) {
		std::cout << "Error: Movie not found.\n";
	}

	std::cout << "Press any key to continue...\n";
	_getch();
}

void User::bookSeatsForShow(const std::string& userId, int movieId, int showId)
{
	system("cls");
	try {
		Movie movie = MovieManager::getMovieById(movieId);
		Show show = MovieManager::getShowById(showId);

		std::cout << "=== Booking for: " << movie.getTitle() << " ===\n";
		std::cout << "Show Time: " << show.showTime << "\n";
		std::cout << "Cinema ID: " << show.cinemaId << " | Hall ID: " << show.hallId << "\n\n";

		// Get or create seat layout
		SeatLayout* layout = SeatManager::getOrCreateSeatLayout(
			show.hallId, show.showId, Config::SEAT_ROWS, Config::SEATS_PER_ROW
		);

		if (!layout) {
			std::cout << "Error: Could not load seat layout.\n";
			std::cout << "Press any key to continue...\n";
			_getch();
			return;
		}

		// Display seat layout
		std::cout << "=== Seat Layout ===\n";
		std::cout << "Legend: [A]=Available  [B]=Booked  [R]=Reserved\n\n";

		auto seatGrid = layout->getLayout();
		for (size_t row = 0; row < seatGrid.size(); row++) {
			std::cout << "Row " << (row + 1) << ": ";
			for (size_t col = 0; col < seatGrid[row].size(); col++) {
				char symbol;
				if (seatGrid[row][col].isAvailable()) {
					symbol = 'A';
				}
				else if (seatGrid[row][col].getStatus() == SeatStatus::BOOKED) {
					symbol = 'B';
				}
				else {
					symbol = 'R';
				}
				std::cout << "[" << symbol << "] ";
			}
			std::cout << "\n";
		}

		std::cout << "\nAvailable seats: " << layout->getAvailableSeatsCount() << "\n";
		std::cout << "Booked seats: " << layout->getBookedSeatsCount() << "\n\n";

		// Book seats
		bool continueBooking = true;
		std::vector<std::pair<int, int>> selectedSeats; // (row, col)
		double totalPrice = 0.0;

		while (continueBooking) {
			std::cout << "Select seat (row col) or type '-1 -1' to finish: ";
			int row, col;
			std::cin >> row >> col;

			if (row == -1 && col == -1) {
				continueBooking = false;
			}
			else if (row < 1 || row > static_cast<int>(seatGrid.size()) || 
					 col < 1 || col > static_cast<int>(seatGrid[0].size())) {
				std::cout << "Invalid seat. Try again.\n";
			}
			else {
				int actualRow = row - 1;
				int actualCol = col - 1;
				Seat& seat = layout->getSeat(actualRow, actualCol);

				if (!seat.isAvailable()) {
					std::cout << "Seat is not available.\n";
				}
				else {
					selectedSeats.push_back({ actualRow, actualCol });
					totalPrice += seat.getPrice();
					std::cout << "Seat selected. Total price so far: " << totalPrice << "\n";
				}
			}
		}

		if (selectedSeats.empty()) {
			std::cout << "No seats selected. Booking cancelled.\n";
			std::cout << "Press any key to continue...\n";
			_getch();
			return;
		}

		// Confirm booking
		std::cout << "\n=== Booking Confirmation ===\n";
		std::cout << "Movie: " << movie.getTitle() << "\n";
		std::cout << "Show Time: " << show.showTime << "\n";
		std::cout << "Seats: " << selectedSeats.size() << "\n";
		std::cout << "Total Price: " << totalPrice << "\n";
		std::cout << "Confirm booking? (y/n): ";

		char confirm;
		std::cin >> confirm;

		if (confirm == 'y' || confirm == 'Y') {
			// Book seats
			for (const auto& seat : selectedSeats) {
				if (!layout->bookSeat(seat.first, seat.second, userId)) {
					std::cout << "Error booking seat at row " << (seat.first + 1) 
							  << ", col " << (seat.second + 1) << "\n";
				}
			}

			// Save seat layout
			layout->saveToFile();

			// Save booking to legacy system
			std::ofstream bookingFile(Config::BOOKINGS_FILE, std::ios::app);
			if (bookingFile) {
				for (const auto& seat : selectedSeats) {
					int seatNumber = seat.first * Config::SEATS_PER_ROW + seat.second + 1;
					bookingFile << userId << "|" << movie.getTitle() << "|" 
							   << show.showTime << "|" << seatNumber << "\n";
				}
			}
			bookingFile.close();

			// Send booking confirmation notification
			NotificationManager::sendBookingConfirmation(userId, 0);

			std::cout << "\n✓ Booking confirmed! Your booking has been saved.\n";
			std::cout << "Press any key to continue...\n";
			_getch();
		}
		else {
			std::cout << "Booking cancelled.\n";
			std::cout << "Press any key to continue...\n";
			_getch();
		}
	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << "\n";
		std::cout << "Press any key to continue...\n";
		_getch();
	}
}

void User::cancelBooking(const std::string& userId)
{
	system("cls");
	auto bookingsLegacy = BookingManager::getUserBookingsLegacy(userId);

	if (bookingsLegacy.empty()) {
		std::cout << "No bookings to cancel.\n";
		std::cout << "Press any key to continue...\n";
		_getch();
		return;
	}

	std::cout << "=== Your Bookings ===\n";
	for (size_t i = 0; i < bookingsLegacy.size(); i++) {
		std::cout << (i + 1) << ". " << bookingsLegacy[i].movieTitle 
				  << " - " << bookingsLegacy[i].showTime 
				  << " - Seat: " << bookingsLegacy[i].seatNumber << "\n";
	}

	std::cout << "\nSelect booking to cancel (0 to skip): ";
	int choice;
	std::cin >> choice;

	if (choice > 0 && choice <= static_cast<int>(bookingsLegacy.size())) {
		auto& booking = bookingsLegacy[choice - 1];
		std::cout << "Cancel booking for " << booking.movieTitle << "? (y/n): ";
		char confirm;
		std::cin >> confirm;

		if (confirm == 'y' || confirm == 'Y') {
			if (BookingManager::cancelBooking(userId, booking.movieTitle, booking.showTime, booking.seatNumber)) {
				std::cout << "Booking cancelled successfully.\n";
				NotificationManager::sendBookingCancellation(userId, 0);
			}
			else {
				std::cout << "Error cancelling booking.\n";
			}
		}
	}

	std::cout << "Press any key to continue...\n";
	_getch();
}

void User::viewNotifications(const std::string& userId)
{
	system("cls");
	auto notifications = NotificationManager::getUserNotifications(userId);

	std::cout << "=== Your Notifications ===\n";
	if (notifications.empty()) {
		std::cout << "No notifications.\n";
	}
	else {
		for (size_t i = 0; i < notifications.size(); i++) {
			std::string status = notifications[i].getIsRead() ? "[Read]" : "[NEW]";
			std::cout << (i + 1) << ". " << status << " " << notifications[i].getMessage() 
					  << "\n    (" << notifications[i].getTimestamp() << ")\n";
		}
	}

	std::cout << "Press any key to continue...\n";
	_getch();
}

void User::manageProfile(const std::string& userId)
{
	int choice = 0;
	do
	{
		system("cls");
		std::cout << "=== Profile Management ===\n";
		std::cout << "1. Update Email\n";
		std::cout << "2. Update Phone\n";
		std::cout << "3. Add/Update Payment Method\n";
		std::cout << "4. Back to User Menu\n";
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
			std::string newEmail;
			std::cout << "=== Update Email ===\n";
			std::cout << "Enter new email: ";
			std::cin.ignore();
			std::getline(std::cin, newEmail);

			if (updateProfile(newEmail, this->phone)) {
				std::cout << "Email updated successfully.\n";
			}
			else {
				std::cout << "Failed to update email.\n";
			}

			std::cout << "Press any key to continue...\n";
			_getch();
			break;
		}
		case 2:
		{
			system("cls");
			std::string newPhone;
			std::cout << "=== Update Phone ===\n";
			std::cout << "Enter new phone: ";
			std::cin.ignore();
			std::getline(std::cin, newPhone);

			if (updateProfile(this->email, newPhone)) {
				std::cout << "Phone updated successfully.\n";
			}
			else {
				std::cout << "Failed to update phone.\n";
			}

			std::cout << "Press any key to continue...\n";
			_getch();
			break;
		}
		case 3:
		{
			system("cls");
			std::string cardNumber, cardholderName;
			std::cout << "=== Add/Update Payment Method ===\n";
			std::cout << "Card Number: ";
			std::cin.ignore();
			std::getline(std::cin, cardNumber);

			std::cout << "Cardholder Name: ";
			std::getline(std::cin, cardholderName);

			if (addPaymentMethod(cardNumber, cardholderName)) {
				std::cout << "Payment method updated successfully.\n";
			}
			else {
				std::cout << "Failed to update payment method.\n";
			}

			std::cout << "Press any key to continue...\n";
			_getch();
			break;
		}
		case 4:
			break;
		default:
			std::cout << "Invalid choice. Press any key to try again...\n";
			_getch();
			break;
		}
	} while (choice != 4);
}

void User::userArea(const std::string& id)
{
	int choice = 0;
	do
	{
		system("cls");
		std::cout << "=== User Area (" << id << ") ===\n";
		std::cout << "1. Browse movies\n";
		std::cout << "2. Search movies\n";
		std::cout << "3. View shows for movie\n";
		std::cout << "4. Book tickets\n";
		std::cout << "5. View my bookings\n";
		std::cout << "6. Cancel booking\n";
		std::cout << "7. View notifications\n";
		std::cout << "8. Manage profile\n";
		std::cout << "9. Logout\n";
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
			auto movies = MovieManager::getAllMovies();
			std::cout << "=== Available Movies ===\n";
			if (movies.empty()) {
				std::cout << "No movies available.\n";
			} else {
				for (size_t i = 0; i < movies.size(); i++) {
					std::cout << (i+1) << ". " << movies[i].getTitle() 
							  << " (" << movies[i].getLanguage() << ")\n";
				}
			}
			std::cout << "Press any key to continue...\n";
			_getch();
			break;
		}
		case 2:
		{
			system("cls");
			std::string searchTerm;
			std::cout << "Enter movie title to search: ";
			std::cin.ignore();
			std::getline(std::cin, searchTerm);

			auto results = MovieManager::searchMovies(searchTerm);
			std::cout << "Search Results:\n";
			if (results.empty()) {
				std::cout << "No movies found.\n";
			} else {
				for (size_t i = 0; i < results.size(); i++) {
					std::cout << (i+1) << ". " << results[i].getTitle() 
							  << " - " << results[i].getGenre() 
							  << " (" << results[i].getReleaseDate() << ")\n";
				}
			}
			std::cout << "Press any key to continue...\n";
			_getch();
			break;
		}
		case 3:
		{
			system("cls");
			auto movies = MovieManager::getAllMovies();
			if (movies.empty()) {
				std::cout << "No movies available.\n";
			}
			else {
				std::cout << "=== Available Movies ===\n";
				for (size_t i = 0; i < movies.size(); i++) {
					std::cout << (i + 1) << ". " << movies[i].getTitle() << "\n";
				}
				std::cout << "Select movie (0 to skip): ";
				int movieChoice;
				std::cin >> movieChoice;

				if (movieChoice > 0 && movieChoice <= static_cast<int>(movies.size())) {
					viewShowsForMovie(movies[movieChoice - 1].getMovieId());
				}
			}
			break;
		}
		case 4:
		{
			system("cls");
			auto movies = MovieManager::getAllMovies();
			if (movies.empty()) {
				std::cout << "No movies available to book.\n";
				std::cout << "Press any key to continue...\n";
				_getch();
			}
			else {
				std::cout << "=== Select Movie to Book ===\n";
				for (size_t i = 0; i < movies.size(); i++) {
					std::cout << (i + 1) << ". " << movies[i].getTitle() << "\n";
				}
				std::cout << "Select movie (0 to skip): ";
				int movieChoice;
				std::cin >> movieChoice;

				if (movieChoice > 0 && movieChoice <= static_cast<int>(movies.size())) {
					int movieId = movies[movieChoice - 1].getMovieId();
					auto shows = MovieManager::getMovieShows(movieId);

					if (shows.empty()) {
						system("cls");
						std::cout << "No shows available for this movie.\n";
						std::cout << "Press any key to continue...\n";
						_getch();
					}
					else {
						system("cls");
						std::cout << "=== Select Show ===\n";
						for (size_t i = 0; i < shows.size(); i++) {
							std::cout << (i + 1) << ". " << shows[i].showTime << "\n";
						}
						std::cout << "Select show (0 to skip): ";
						int showChoice;
						std::cin >> showChoice;

						if (showChoice > 0 && showChoice <= static_cast<int>(shows.size())) {
							bookSeatsForShow(id, movieId, shows[showChoice - 1].showId);
						}
					}
				}
			}
			break;
		}
		case 5:
		{
			system("cls");
			auto bookingsLegacy = BookingManager::getUserBookingsLegacy(id);
			std::cout << "=== Your Bookings ===\n";
			if (bookingsLegacy.empty()) {
				std::cout << "No bookings found.\n";
			} else {
				for (size_t i = 0; i < bookingsLegacy.size(); i++) {
					std::cout << (i+1) << ". " << bookingsLegacy[i].movieTitle 
							  << " - " << bookingsLegacy[i].showTime 
							  << " - Seat: " << bookingsLegacy[i].seatNumber << "\n";
				}
			}
			std::cout << "Press any key to continue...\n";
			_getch();
			break;
		}
		case 6:
		{
			cancelBooking(id);
			break;
		}
		case 7:
		{
			viewNotifications(id);
			break;
		}
		case 8:
		{
			manageProfile(id);
			break;
		}
		case 9:
			std::cout << "Logging out...\n";
			break;
		default:
			std::cout << "Invalid choice. Press any key to try again...\n";
			_getch();
			break;
		}
	} while (choice != 9);
}
