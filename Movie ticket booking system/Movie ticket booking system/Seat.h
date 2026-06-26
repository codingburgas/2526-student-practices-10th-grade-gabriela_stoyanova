#pragma once
#ifndef SEAT_H
#define SEAT_H

#include "MovieEnhanced.h"
#include <string>
#include <vector>

enum class SeatStatus {
	AVAILABLE,
	BOOKED,
	RESERVED_TEMPORARY
};

class Seat {
private:
	int seatNumber;
	SeatType seatType;
	SeatStatus status;
	std::string bookedBy;
	long long reservationTimestamp;

public:
	Seat(int number, SeatType type);

	int getSeatNumber() const;
	SeatType getSeatType() const;
	SeatStatus getStatus() const;
	std::string getBookedBy() const;
	double getPrice() const;

	bool isAvailable() const;
	bool book(const std::string& userId);
	bool reserve(const std::string& userId);
	bool cancel();
	bool releaseIfExpired(long long timeoutSeconds);
};

class SeatLayout {
private:
	int hallId;
	int showId;
	std::vector<std::vector<Seat>> seats;
	int rows;
	int seatsPerRow;

public:
	SeatLayout(int hallId, int showId, int rows, int seatsPerRow);

	bool bookSeat(int row, int col, const std::string& userId);
	bool reserveSeat(int row, int col, const std::string& userId);
	bool cancelSeatBooking(int row, int col);
	Seat& getSeat(int row, int col);
	const Seat& getSeatConst(int row, int col) const;
	std::vector<std::vector<Seat>> getLayout() const;
	int getAvailableSeatsCount() const;
	int getBookedSeatsCount() const;

	bool loadFromFile();
	bool saveToFile();
};

namespace SeatManager {
	bool initializeShowSeats(int hallId, int showId, int rows, int seatsPerRow);
	SeatLayout* getOrCreateSeatLayout(int hallId, int showId, int rows, int seatsPerRow);
	std::string getLayoutFilePath(int hallId, int showId);
}

#endif // SEAT_H
