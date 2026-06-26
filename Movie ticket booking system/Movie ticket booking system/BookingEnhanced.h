#pragma once
#ifndef BOOKING_ENHANCED_H
#define BOOKING_ENHANCED_H

#include "MovieEnhanced.h"
#include <string>
#include <vector>

enum class BookingStatus {
	CONFIRMED,
	CANCELLED,
	PENDING_PAYMENT
};

enum class PaymentMethod {
	CREDIT_CARD,
	CASH,
	ONLINE
};

enum class BookingType {
	ONLINE,
	WALK_IN
};

struct Ticket {
	int ticketId;
	int bookingId;
	int seatNumber;
	int row;
	int col;
	SeatType seatType;
	double price;
};

class Booking {
private:
	int bookingId;
	std::string userId;
	int showId;
	BookingStatus status;
	BookingType bookingType;
	PaymentMethod paymentMethod;
	std::vector<Ticket> tickets;
	double totalAmount;
	std::string bookingTime;

public:
	Booking(const std::string& userId, int showId, BookingType type);

	int getBookingId() const;
	std::string getUserId() const;
	int getShowId() const;
	BookingStatus getStatus() const;
	BookingType getBookingType() const;
	PaymentMethod getPaymentMethod() const;
	void addTicket(const Ticket& ticket);
	const std::vector<Ticket>& getTickets() const;
	double getTotalAmount() const;
	std::string getBookingTime() const;

	bool confirmBooking(PaymentMethod method);
	bool cancelBooking();
	bool processPayment(PaymentMethod method);
	int getTicketCount() const;
};

namespace BookingManager {
	// Booking operations
	int createBooking(const std::string& userId, int showId, BookingType type);
	bool addTicketToBooking(int bookingId, const Ticket& ticket);
	bool confirmBooking(int bookingId, PaymentMethod method);
	bool cancelBooking(int bookingId);

	// Seat operations
	std::vector<int> getAvailableSeats(int showId);
	std::vector<int> getBookedSeats(int showId);
	bool reserveSeatTemporary(int showId, int seatNumber, const std::string& userId);
	bool bookSeat(int bookingId, int seatNumber);

	// Query operations
	std::vector<Booking> getUserBookings(const std::string& userId);
	Booking getBookingDetails(int bookingId);

	// File operations
	bool loadBookingsFromFile();
	bool saveBookingToFile(const Booking& booking);
}

#endif // BOOKING_ENHANCED_H
