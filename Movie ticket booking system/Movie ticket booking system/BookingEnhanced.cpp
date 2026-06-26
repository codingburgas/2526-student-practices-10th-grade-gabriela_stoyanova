#include "BookingEnhanced.h"
#include "Seat.h"
#include "Config.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>
#include <algorithm>

// Booking implementation
static int bookingIdCounter = 1000;

Booking::Booking(const std::string& userId, int showId, BookingType type)
	: bookingId(bookingIdCounter++), userId(userId), showId(showId), 
	  status(BookingStatus::PENDING_PAYMENT), bookingType(type), 
	  paymentMethod(PaymentMethod::CREDIT_CARD), totalAmount(0.0) {

	// Get current timestamp
	std::time_t now = std::time(nullptr);
	char buffer[100];
	struct tm timeinfo;
	localtime_s(&timeinfo, &now);
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
	bookingTime = buffer;
}

int Booking::getBookingId() const { return bookingId; }
std::string Booking::getUserId() const { return userId; }
int Booking::getShowId() const { return showId; }
BookingStatus Booking::getStatus() const { return status; }
BookingType Booking::getBookingType() const { return bookingType; }
PaymentMethod Booking::getPaymentMethod() const { return paymentMethod; }
double Booking::getTotalAmount() const { return totalAmount; }
std::string Booking::getBookingTime() const { return bookingTime; }
int Booking::getTicketCount() const { return tickets.size(); }

void Booking::addTicket(const Ticket& ticket) {
	tickets.push_back(ticket);
	totalAmount += ticket.price;
}

const std::vector<Ticket>& Booking::getTickets() const {
	return tickets;
}

bool Booking::confirmBooking(PaymentMethod method) {
	if (tickets.empty()) {
		return false;
	}
	paymentMethod = method;
	if (processPayment(method)) {
		status = BookingStatus::CONFIRMED;
		return true;
	}
	return false;
}

bool Booking::cancelBooking() {
	status = BookingStatus::CANCELLED;
	return true;
}

bool Booking::processPayment(PaymentMethod method) {
	switch (method) {
		case PaymentMethod::CREDIT_CARD:
		case PaymentMethod::CASH:
		case PaymentMethod::ONLINE:
			return true; // Simplified payment processing
		default:
			return false;
	}
}

// BookingManager namespace implementation
static std::vector<Booking> bookingDatabase;

namespace BookingManager {

int createBooking(const std::string& userId, int showId, BookingType type)
{
	Booking booking(userId, showId, type);
	int id = booking.getBookingId();
	bookingDatabase.push_back(booking);
	return id;
}

bool addTicketToBooking(int bookingId, const Ticket& ticket)
{
	for (auto& booking : bookingDatabase) {
		if (booking.getBookingId() == bookingId) {
			booking.addTicket(ticket);
			return true;
		}
	}
	return false;
}

bool confirmBooking(int bookingId, PaymentMethod method)
{
	for (auto& booking : bookingDatabase) {
		if (booking.getBookingId() == bookingId) {
			return booking.confirmBooking(method);
		}
	}
	return false;
}

bool cancelBooking(int bookingId)
{
	for (auto& booking : bookingDatabase) {
		if (booking.getBookingId() == bookingId) {
			booking.cancelBooking();
			return true;
		}
	}
	return false;
}

std::vector<int> getAvailableSeats(int showId)
{
	std::vector<int> available;
	// This would require knowing the show's hall to get seat layout
	// Placeholder implementation
	return available;
}

std::vector<int> getBookedSeats(int showId)
{
	std::vector<int> booked;
	// Placeholder implementation
	return booked;
}

bool reserveSeatTemporary(int showId, int seatNumber, const std::string& userId)
{
	// Placeholder implementation
	return true;
}

bool bookSeat(int bookingId, int seatNumber)
{
	// Placeholder implementation
	return true;
}

std::vector<Booking> getUserBookings(const std::string& userId)
{
	std::vector<Booking> userBookings;
	for (const auto& booking : bookingDatabase) {
		if (booking.getUserId() == userId) {
			userBookings.push_back(booking);
		}
	}
	return userBookings;
}

Booking getBookingDetails(int bookingId)
{
	for (const auto& booking : bookingDatabase) {
		if (booking.getBookingId() == bookingId) {
			return booking;
		}
	}
	throw std::runtime_error("Booking not found");
}

bool loadBookingsFromFile()
{
	std::ifstream file(Config::BOOKINGS_FILE);
	if (!file) return false;

	// Placeholder implementation
	file.close();
	return true;
}

bool saveBookingToFile(const Booking& booking)
{
	std::ofstream file(Config::BOOKINGS_FILE, std::ios::app);
	if (!file) return false;

	file << booking.getBookingId() << " "
		 << booking.getUserId() << " "
		 << booking.getShowId() << " "
		 << static_cast<int>(booking.getStatus()) << " "
		 << static_cast<int>(booking.getBookingType()) << " "
		 << booking.getTotalAmount() << " "
		 << booking.getBookingTime() << "\n";

	file.close();
	return true;
}

}
