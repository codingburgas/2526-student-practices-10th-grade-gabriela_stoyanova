#pragma once
#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>
#include <vector>

enum class NotificationType {
	NEW_MOVIE_RELEASE,
	BOOKING_CONFIRMATION,
	BOOKING_CANCELLATION
};

class Notification {
private:
	int notificationId;
	std::string userId;
	NotificationType type;
	std::string message;
	std::string timestamp;
	bool isRead;

public:
	Notification(const std::string& userId, NotificationType type, const std::string& msg);

	int getNotificationId() const;
	std::string getUserId() const;
	NotificationType getType() const;
	std::string getMessage() const;
	std::string getTimestamp() const;
	bool getIsRead() const;
	void markAsRead();
};

namespace NotificationManager {
	bool sendBookingConfirmation(const std::string& userId, int bookingId);
	bool sendBookingCancellation(const std::string& userId, int bookingId);
	bool sendNewMovieNotification(const std::string& movieTitle);

	std::vector<Notification> getUserNotifications(const std::string& userId);
	bool markNotificationAsRead(int notificationId);

	// File operations
	bool loadNotificationsFromFile();
	bool saveNotificationToFile(const Notification& notification);
}

#endif // NOTIFICATION_H
