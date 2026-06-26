#pragma once
#ifndef CINEMA_H
#define CINEMA_H

#include <string>
#include <vector>
#include <memory>

class CinemaHall {
private:
	int hallId;
	int totalSeats;
	std::string hallName;

public:
	CinemaHall(int id, const std::string& name, int seats);
	int getHallId() const;
	std::string getHallName() const;
	int getTotalSeats() const;
};

class Cinema {
private:
	int cinemaId;
	std::string cinemaName;
	std::string city;
	std::string address;
	std::vector<std::shared_ptr<CinemaHall>> halls;

public:
	Cinema(int id, const std::string& name, const std::string& city, const std::string& address);
	int getCinemaId() const;
	std::string getCinemaName() const;
	std::string getCity() const;
	std::string getAddress() const;
	void addHall(std::shared_ptr<CinemaHall> hall);
	const std::vector<std::shared_ptr<CinemaHall>>& getHalls() const;
};

namespace CinemaManager {
	bool addCinema(const Cinema& cinema);
	bool deleteCinema(int cinemaId);
	std::vector<Cinema> getAllCinemas();
	std::vector<Cinema> getCinemasByCity(const std::string& city);
	bool loadCinemasFromFile();
	bool saveCinemaToFile(const Cinema& cinema);
}

#endif // CINEMA_H
