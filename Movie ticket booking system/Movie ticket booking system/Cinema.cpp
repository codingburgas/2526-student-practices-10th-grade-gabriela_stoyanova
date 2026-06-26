#include "Cinema.h"
#include "Config.h"
#include <fstream>
#include <iostream>
#include <sstream>

// CinemaHall implementation
CinemaHall::CinemaHall(int id, const std::string& name, int seats)
	: hallId(id), hallName(name), totalSeats(seats) {}

int CinemaHall::getHallId() const { return hallId; }
std::string CinemaHall::getHallName() const { return hallName; }
int CinemaHall::getTotalSeats() const { return totalSeats; }

// Cinema implementation
Cinema::Cinema(int id, const std::string& name, const std::string& city, const std::string& address)
	: cinemaId(id), cinemaName(name), city(city), address(address) {}

int Cinema::getCinemaId() const { return cinemaId; }
std::string Cinema::getCinemaName() const { return cinemaName; }
std::string Cinema::getCity() const { return city; }
std::string Cinema::getAddress() const { return address; }

void Cinema::addHall(std::shared_ptr<CinemaHall> hall) {
	halls.push_back(hall);
}

const std::vector<std::shared_ptr<CinemaHall>>& Cinema::getHalls() const {
	return halls;
}

// CinemaManager namespace implementation
static std::vector<Cinema> cinemaDatabase;

namespace CinemaManager {

bool addCinema(const Cinema& cinema)
{
	cinemaDatabase.push_back(cinema);
	return saveCinemaToFile(cinema);
}

bool deleteCinema(int cinemaId)
{
	auto it = cinemaDatabase.begin();
	while (it != cinemaDatabase.end()) {
		if (it->getCinemaId() == cinemaId) {
			cinemaDatabase.erase(it);
			return true;
		}
		++it;
	}
	return false;
}

std::vector<Cinema> getAllCinemas()
{
	return cinemaDatabase;
}

std::vector<Cinema> getCinemasByCity(const std::string& city)
{
	std::vector<Cinema> result;
	for (const auto& cinema : cinemaDatabase) {
		if (cinema.getCity() == city) {
			result.push_back(cinema);
		}
	}
	return result;
}

bool loadCinemasFromFile()
{
	std::ifstream file(Config::CINEMAS_FILE);
	if (!file) return false;

	int id, hallCount, hallId, seats;
	std::string name, city, address, hallName;

	while (file >> id >> name >> city >> address >> hallCount) {
		Cinema cinema(id, name, city, address);

		for (int i = 0; i < hallCount; i++) {
			file >> hallId >> hallName >> seats;
			cinema.addHall(std::make_shared<CinemaHall>(hallId, hallName, seats));
		}

		cinemaDatabase.push_back(cinema);
	}

	file.close();
	return true;
}

bool saveCinemaToFile(const Cinema& cinema)
{
	std::ofstream file(Config::CINEMAS_FILE, std::ios::app);
	if (!file) return false;

	file << cinema.getCinemaId() << " "
		 << cinema.getCinemaName() << " "
		 << cinema.getCity() << " "
		 << cinema.getAddress() << " "
		 << cinema.getHalls().size() << "\n";

	for (const auto& hall : cinema.getHalls()) {
		file << hall->getHallId() << " "
			 << hall->getHallName() << " "
			 << hall->getTotalSeats() << "\n";
	}

	file.close();
	return true;
}

}
