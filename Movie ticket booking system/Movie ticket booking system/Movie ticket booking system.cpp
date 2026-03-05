#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void printDesign(string fileName) {
    ifstream file(fileName);
    if (file.is_open()) {
        cout << file.rdbuf(); 
        file.close();
    }
    else {

        cout << "--- MOVIE TICKET BOOKING SYSTEM ---" << endl;
        cout << "[Error: " << fileName << " not found]" << endl;
    }
}

int main() {
    int choice;

    while (true) {
        system("cls");

        printDesign("landpage.txt");

        cout << "\n\n   Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            break;
        case 2:
            break;
        case 3:
            return 0;
        default:
            cout << "Invalid selection!";
            system("pause");
        }
    }
    return 0;
}