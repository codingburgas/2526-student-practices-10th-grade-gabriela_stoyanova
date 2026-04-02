#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <conio.h>
#include "Administrator.h"

using namespace std;

void Administrator::changePass(const std::string& currentId)
{
    cout << "Enter new admin user: " << endl;
	cin >> adminId;
	cout << "Enter new password: " << endl;
	cin >> adminPass;

	// If currentId provided, update that admin entry; otherwise append as new admin
	std::vector<std::pair<std::string, std::string>> admins;
	std::ifstream in("admins.txt");
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

	std::ofstream out("admins.txt", std::ios::out | std::ios::trunc);
	if (!out)
	{
		cerr << "Error: could not open 'admins.txt' for writing." << endl;
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
	std::ifstream adminData("admins.txt");
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
	std::ifstream in("admins.txt");
	return in.good() && !in.eof();
}

bool Administrator::registerAdmin(const std::string& id, const std::string& pass)
{
	std::ofstream out("admins.txt", std::ios::app);
	if (!out)
		return false;
	out << id << " " << pass << "\n";
	return true;
}

bool Administrator::loginAdmin(const std::string& id, const std::string& pass)
{
	return verifyCredentials(id, pass);
}
