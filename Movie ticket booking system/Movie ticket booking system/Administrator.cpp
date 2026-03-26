#include <string>
#include <iostream>
#include <fstream>
#include <conio.h>
#include "Administrator.h"
using namespace std;

void Administrator::changePass()
{
	cout << "Enter new admin user: " << endl;
	cin >> adminId;
	cout << "Enter new password: " << endl;
	cin >> adminPass;
	ofstream adminData("admins.txt");
	{
		adminData << adminId << " " << adminPass << endl;
	}
	adminData.close();
	cout << "Changes saved" << endl;
	cout << "Press any key to continue..." << endl;
	_getch();
	system("cls");
}
