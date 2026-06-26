#pragma once
#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

class User {
private:
    std::string email;
    std::string phone;
    std::string cardNumber;

public:
    bool hasAnyUsers();
    bool registerUser(const std::string& id, const std::string& pass);
    bool loginUser(const std::string& id, const std::string& pass);
    void userArea(const std::string& id);

    // Profile and payment methods
    bool addPaymentMethod(const std::string& cardNumber, const std::string& cardholderName);
    bool updateProfile(const std::string& email, const std::string& phone);

    // Booking and show methods
    void viewShowsForMovie(int movieId);
    void bookSeatsForShow(const std::string& userId, int movieId, int showId);
    void cancelBooking(const std::string& userId);
    void viewNotifications(const std::string& userId);
    void manageProfile(const std::string& userId);
};

#endif // USER_H
