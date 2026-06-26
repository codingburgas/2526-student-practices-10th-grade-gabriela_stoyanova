#pragma once
#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include <string>

class Administrator {
public:
	void changePass(const std::string& currentId = "");
	bool hasAnyAdmins();
	bool registerAdmin(const std::string& id, const std::string& pass);
	bool loginAdmin(const std::string& id, const std::string& pass);
	bool verifyCredentials(const std::string& id, const std::string& pass);
	int getUserCount();
	void adminArea(const std::string& id);

	// Cinema and show management methods
	void manageCinemas();
	void manageMoviesAndShows();

	// Report and analytics methods
	void viewBookingReports();
	void viewRevenueReports();

	// User management
	void manageUsers();

private:
	std::string adminId;
	std::string adminPass;
};

#endif
