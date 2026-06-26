#pragma once
#ifndef MOVIE_ENHANCED_H
#define MOVIE_ENHANCED_H

#include <string>
#include <vector>
#include <memory>

enum class SeatType {
	SILVER,
	GOLD,
	PLATINUM
};

struct Show {
	int showId;
	int movieId;
	int cinemaId;
	int hallId;
	std::string showTime;
	double silverPrice;
	double goldPrice;
	double platinumPrice;
	bool isActive;
};

class Movie {
private:
	int movieId;
	std::string title;
	std::string language;
	std::string genre;
	std::string releaseDate;
	std::string description;
	std::string director;
	int durationMinutes;

public:
	Movie(int id, const std::string& title, const std::string& lang, 
		  const std::string& genre, const std::string& releaseDate);

	int getMovieId() const;
	std::string getTitle() const;
	std::string getLanguage() const;
	std::string getGenre() const;
	std::string getReleaseDate() const;
	std::string getDescription() const;
	std::string getDirector() const;
	int getDurationMinutes() const;

	void setDescription(const std::string& desc);
	void setDirector(const std::string& dir);
	void setDuration(int duration);
};

namespace MovieManager {
	// Movie operations
	bool addMovie(const Movie& movie);
	bool deleteMovie(int movieId);
	std::vector<Movie> getAllMovies();
	std::vector<Movie> searchMovies(const std::string& title = "", 
									 const std::string& language = "", 
									 const std::string& genre = "", 
									 const std::string& releaseDate = "");
	Movie getMovieById(int movieId);

	// Show operations
	bool addShow(const Show& show);
	bool deleteShow(int showId);
	bool updateShow(int showId, const Show& updatedShow);
	std::vector<Show> getMovieShows(int movieId);
	std::vector<Show> getShowsByHall(int cinemaId, int hallId);
	std::vector<Show> getShowsByCity(const std::string& city);
	Show getShowById(int showId);

	// File operations
	bool loadMoviesFromFile();
	bool loadShowsFromFile();
	bool saveMovieToFile(const Movie& movie);
	bool saveShowToFile(const Show& show);
}

#endif // MOVIE_ENHANCED_H
