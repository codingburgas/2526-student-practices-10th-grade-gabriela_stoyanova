#include "Seat.h"
#include "Config.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>
#include <map>

// Seat implementation
Seat::Seat(int number, SeatType type)
	: seatNumber(number), seatType(type), status(SeatStatus::AVAILABLE), 
	  reservationTimestamp(0) {}

int Seat::getSeatNumber() const { return seatNumber; }
SeatType Seat::getSeatType() const { return seatType; }
SeatStatus Seat::getStatus() const { return status; }
std::string Seat::getBookedBy() const { return bookedBy; }

double Seat::getPrice() const {
	switch (seatType) {
		case SeatType::SILVER:
			return 150.0;
		case SeatType::GOLD:
			return 200.0;
		case SeatType::PLATINUM:
			return 300.0;
		default:
			return 0.0;
	}
}

bool Seat::isAvailable() const {
	return status == SeatStatus::AVAILABLE;
}

bool Seat::book(const std::string& userId) {
	if (status != SeatStatus::AVAILABLE) {
		return false;
	}
	bookedBy = userId;
	status = SeatStatus::BOOKED;
	return true;
}

bool Seat::reserve(const std::string& userId) {
	if (status != SeatStatus::AVAILABLE) {
		return false;
	}
	bookedBy = userId;
	status = SeatStatus::RESERVED_TEMPORARY;
	reservationTimestamp = std::time(nullptr);
	return true;
}

bool Seat::cancel() {
	if (status == SeatStatus::BOOKED || status == SeatStatus::RESERVED_TEMPORARY) {
		status = SeatStatus::AVAILABLE;
		bookedBy.clear();
		reservationTimestamp = 0;
		return true;
	}
	return false;
}

bool Seat::releaseIfExpired(long long timeoutSeconds) {
	if (status == SeatStatus::RESERVED_TEMPORARY) {
		long long currentTime = std::time(nullptr);
		if (currentTime - reservationTimestamp > timeoutSeconds) {
			cancel();
			return true;
		}
	}
	return false;
}

// SeatLayout implementation
SeatLayout::SeatLayout(int hallId, int showId, int rows, int seatsPerRow)
	: hallId(hallId), showId(showId), rows(rows), seatsPerRow(seatsPerRow) {

	// Initialize seat grid
	for (int r = 0; r < rows; r++) {
		std::vector<Seat> row;
		for (int c = 0; c < seatsPerRow; c++) {
			int seatNumber = r * seatsPerRow + c + 1;
			// Vary seat types: Silver, Gold, Platinum pattern
			SeatType type = SeatType::SILVER;
			if (c >= seatsPerRow * 2 / 3) {
				type = SeatType::PLATINUM;
			} else if (c >= seatsPerRow / 3) {
				type = SeatType::GOLD;
			}
			row.emplace_back(seatNumber, type);
		}
		seats.push_back(row);
	}
}

bool SeatLayout::bookSeat(int row, int col, const std::string& userId) {
	if (row < 0 || row >= rows || col < 0 || col >= seatsPerRow) {
		return false;
	}
	return seats[row][col].book(userId);
}

bool SeatLayout::reserveSeat(int row, int col, const std::string& userId) {
	if (row < 0 || row >= rows || col < 0 || col >= seatsPerRow) {
		return false;
	}
	return seats[row][col].reserve(userId);
}

bool SeatLayout::cancelSeatBooking(int row, int col) {
	if (row < 0 || row >= rows || col < 0 || col >= seatsPerRow) {
		return false;
	}
	return seats[row][col].cancel();
}

Seat& SeatLayout::getSeat(int row, int col) {
	return seats[row][col];
}

const Seat& SeatLayout::getSeatConst(int row, int col) const {
	return seats[row][col];
}

std::vector<std::vector<Seat>> SeatLayout::getLayout() const {
	return seats;
}

int SeatLayout::getAvailableSeatsCount() const {
	int count = 0;
	for (const auto& row : seats) {
		for (const auto& seat : row) {
			if (seat.isAvailable()) {
				count++;
			}
		}
	}
	return count;
}

int SeatLayout::getBookedSeatsCount() const {
	int count = 0;
	for (const auto& row : seats) {
		for (const auto& seat : row) {
			if (seat.getStatus() == SeatStatus::BOOKED) {
				count++;
			}
		}
	}
	return count;
}

bool SeatLayout::loadFromFile() {
	std::string filepath = SeatManager::getLayoutFilePath(hallId, showId);
	std::ifstream file(filepath);
	if (!file) return false;

	int seatNum;
	int typeInt, statusInt;
	std::string bookedByStr;

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < seatsPerRow; c++) {
			file >> seatNum >> typeInt >> statusInt >> bookedByStr;
			if (statusInt == 1) { // BOOKED
				seats[r][c].book(bookedByStr);
			}
		}
	}

	file.close();
	return true;
}

bool SeatLayout::saveToFile() {
	std::string filepath = SeatManager::getLayoutFilePath(hallId, showId);
	std::ofstream file(filepath);
	if (!file) return false;

	for (const auto& row : seats) {
		for (const auto& seat : row) {
			file << seat.getSeatNumber() << " "
				 << static_cast<int>(seat.getSeatType()) << " "
				 << static_cast<int>(seat.getStatus()) << " "
				 << (seat.getBookedBy().empty() ? "none" : seat.getBookedBy()) << "\n";
		}
	}

	file.close();
	return true;
}

// SeatManager namespace implementation
static std::map<std::string, SeatLayout*> seatLayouts;

namespace SeatManager {

bool initializeShowSeats(int hallId, int showId, int rows, int seatsPerRow) {
	std::string key = std::to_string(hallId) + "_" + std::to_string(showId);
	if (seatLayouts.find(key) != seatLayouts.end()) {
		return true; // Already initialized
	}

	SeatLayout* layout = new SeatLayout(hallId, showId, rows, seatsPerRow);
	seatLayouts[key] = layout;
	return layout->saveToFile();
}

SeatLayout* getOrCreateSeatLayout(int hallId, int showId, int rows, int seatsPerRow) {
	std::string key = std::to_string(hallId) + "_" + std::to_string(showId);

	if (seatLayouts.find(key) != seatLayouts.end()) {
		return seatLayouts[key];
	}

	SeatLayout* layout = new SeatLayout(hallId, showId, rows, seatsPerRow);
	layout->loadFromFile();
	seatLayouts[key] = layout;
	return layout;
}

std::string getLayoutFilePath(int hallId, int showId) {
	return Config::SEATS_DIR + "hall_" + std::to_string(hallId) + 
		   "_show_" + std::to_string(showId) + ".txt";
}

}
