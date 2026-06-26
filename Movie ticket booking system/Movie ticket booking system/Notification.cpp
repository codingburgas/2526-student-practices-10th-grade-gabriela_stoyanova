#include "Notification.h"
#include "Config.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <sstream>
#include <algorithm>

// Notification implementation
static int notificationIdCounter = 5000;

Notification::Notification(const std::string& userId, NotificationType type, const std::string& msg)
	: notificationId(notificationIdCounter++), userId(userId), type(type), 
	  message(msg), isRead(false) {

	// Get current timestamp
	std::time_t now = std::time(nullptr);
	char buffer[100];
	struct tm timeinfo;
	localtime_s(&timeinfo, &now);
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
	timestamp = buffer;
}

int Notification::getNotificationId() const { return notificationId; }
std::string Notification::getUserId() const { return userId; }
NotificationType Notification::getType() const { return type; }
std::string Notification::getMessage() const { return message; }
std::string Notification::getTimestamp() const { return timestamp; }
bool Notification::getIsRead() const { return isRead; }

void Notification::markAsRead() {
	isRead = true;
}

// NotificationManager namespace implementation
static std::vector<Notification> notificationDatabase;

namespace NotificationManager {

bool sendBookingConfirmation(const std::string& userId, int bookingId)
{
	std::string message = "Your booking #" + std::to_string(bookingId) + " has been confirmed.";
	Notification notif(userId, NotificationType::BOOKING_CONFIRMATION, message);
	notificationDatabase.push_back(notif);
	return saveNotificationToFile(notif);
}

bool sendBookingCancellation(const std::string& userId, int bookingId)
{
	std::string message = "Your booking #" + std::to_string(bookingId) + " has been cancelled.";
	Notification notif(userId, NotificationType::BOOKING_CANCELLATION, message);
	notificationDatabase.push_back(notif);
	return saveNotificationToFile(notif);
}

bool sendNewMovieNotification(const std::string& movieTitle)
{
	// In a real system, this would send to all users
	// For now, we'll just log it
	std::string message = "New movie released: " + movieTitle;
	std::cout << "Notification: " << message << std::endl;
	return true;
}

std::vector<Notification> getUserNotifications(const std::string& userId)
{
	std::vector<Notification> userNotifs;
	for (const auto& notif : notificationDatabase) {
		if (notif.getUserId() == userId) {
			userNotifs.push_back(notif);
		}
	}
	return userNotifs;
}

bool markNotificationAsRead(int notificationId)
{
	for (auto& notif : notificationDatabase) {
		if (notif.getNotificationId() == notificationId) {
			notif.markAsRead();
			return true;
		}
	}
	return false;
}

bool loadNotificationsFromFile()
{
	std::ifstream file(Config::NOTIFICATIONS_FILE);
	if (!file) return false;

	int id, typeInt;
	bool isRead;
	std::string userId, message, timestamp;

	while (file >> id >> userId >> typeInt >> isRead) {
		file.ignore();
		std::getline(file, message, '|');
		std::getline(file, timestamp);

		Notification notif(userId, static_cast<NotificationType>(typeInt), message);
		if (isRead) {
			notif.markAsRead();
		}
		notificationDatabase.push_back(notif);
	}

	file.close();
	return true;
}

bool saveNotificationToFile(const Notification& notification)
{
	std::ofstream file(Config::NOTIFICATIONS_FILE, std::ios::app);
	if (!file) return false;

	file << notification.getNotificationId() << " "
		 << notification.getUserId() << " "
		 << static_cast<int>(notification.getType()) << " "
		 << notification.getIsRead() << "\n"
		 << notification.getMessage() << "|"
		 << notification.getTimestamp() << "\n";

	file.close();
	return true;
}

}
