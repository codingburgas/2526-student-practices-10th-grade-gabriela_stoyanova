#include "MovieEnhanced.h"
#include "Config.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

// Movie implementation
Movie::Movie(int id, const std::string& title, const std::string& lang, 
			 const std::string& genre, const std::string& releaseDate)
	: movieId(id), title(title), language(lang), genre(genre), 
	  releaseDate(releaseDate), durationMinutes(0) {}

int Movie::getMovieId() const { return movieId; }
std::string Movie::getTitle() const { return title; }
std::string Movie::getLanguage() const { return language; }
std::string Movie::getGenre() const { return genre; }
std::string Movie::getReleaseDate() const { return releaseDate; }
std::string Movie::getDescription() const { return description; }
std::string Movie::getDirector() const { return director; }
int Movie::getDurationMinutes() const { return durationMinutes; }

void Movie::setDescription(const std::string& desc) { description = desc; }
void Movie::setDirector(const std::string& dir) { director = dir; }
void Movie::setDuration(int duration) { durationMinutes = duration; }

// MovieManager namespace implementation
static std::vector<Movie> movieDatabase;
static std::vector<Show> showDatabase;

namespace MovieManager {

bool addMovie(const Movie& movie)
{
	movieDatabase.push_back(movie);
	return saveMovieToFile(movie);
}

bool deleteMovie(int movieId)
{
	auto it = movieDatabase.begin();
	while (it != movieDatabase.end()) {
		if (it->getMovieId() == movieId) {
			movieDatabase.erase(it);
			return true;
		}
		++it;
	}
	return false;
}

std::vector<Movie> getAllMovies()
{
	return movieDatabase;
}

std::vector<Movie> searchMovies(const std::string& title, 
								const std::string& language, 
								const std::string& genre, 
								const std::string& releaseDate)
{
	std::vector<Movie> results;

	for (const auto& movie : movieDatabase) {
		bool matches = true;

		if (!title.empty() && movie.getTitle().find(title) == std::string::npos)
			matches = false;
		if (!language.empty() && movie.getLanguage() != language)
			matches = false;
		if (!genre.empty() && movie.getGenre() != genre)
			matches = false;
		if (!releaseDate.empty() && movie.getReleaseDate() != releaseDate)
			matches = false;

		if (matches)
			results.push_back(movie);
	}

	return results;
}

Movie getMovieById(int movieId)
{
	for (const auto& movie : movieDatabase) {
		if (movie.getMovieId() == movieId) {
			return movie;
		}
	}
	throw std::runtime_error("Movie not found");
}

bool addShow(const Show& show)
{
	showDatabase.push_back(show);
	return saveShowToFile(show);
}

bool deleteShow(int showId)
{
	auto it = showDatabase.begin();
	while (it != showDatabase.end()) {
		if (it->showId == showId) {
			showDatabase.erase(it);
			return true;
		}
		++it;
	}
	return false;
}

bool updateShow(int showId, const Show& updatedShow)
{
	for (auto& show : showDatabase) {
		if (show.showId == showId) {
			show = updatedShow;
			return true;
		}
	}
	return false;
}

std::vector<Show> getMovieShows(int movieId)
{
	std::vector<Show> results;
	for (const auto& show : showDatabase) {
		if (show.movieId == movieId && show.isActive) {
			results.push_back(show);
		}
	}
	return results;
}

std::vector<Show> getShowsByHall(int cinemaId, int hallId)
{
	std::vector<Show> results;
	for (const auto& show : showDatabase) {
		if (show.cinemaId == cinemaId && show.hallId == hallId && show.isActive) {
			results.push_back(show);
		}
	}
	return results;
}

std::vector<Show> getShowsByCity(const std::string& city)
{
	// This would need Cinema information - stub for now
	return showDatabase;
}

Show getShowById(int showId)
{
	for (const auto& show : showDatabase) {
		if (show.showId == showId) {
			return show;
		}
	}
	throw std::runtime_error("Show not found");
}

bool loadMoviesFromFile()
{
	std::ifstream file(Config::MOVIES_FILE);
	if (!file) return false;

	int id, duration;
	std::string title, language, genre, releaseDate, description, director;

	while (std::getline(file, title, '|')) {
		if (title.empty()) break;

		file >> id >> language >> genre >> releaseDate >> duration;
		file.ignore();
		std::getline(file, description, '|');
		std::getline(file, director);

		Movie movie(id, title, language, genre, releaseDate);
		movie.setDescription(description);
		movie.setDirector(director);
		movie.setDuration(duration);

		movieDatabase.push_back(movie);
	}

	file.close();
	return true;
}

bool loadShowsFromFile()
{
	std::ifstream file(Config::SHOWS_FILE);
	if (!file) return false;

	Show show{};
	while (file >> show.showId >> show.movieId >> show.cinemaId >> show.hallId
				>> show.showTime >> show.silverPrice >> show.goldPrice 
				>> show.platinumPrice >> show.isActive) {
		showDatabase.push_back(show);
	}

	file.close();
	return true;
}

bool saveMovieToFile(const Movie& movie)
{
	std::ofstream file(Config::MOVIES_FILE, std::ios::app);
	if (!file) return false;

	file << movie.getTitle() << "|"
		 << movie.getMovieId() << " "
		 << movie.getLanguage() << " "
		 << movie.getGenre() << " "
		 << movie.getReleaseDate() << " "
		 << movie.getDurationMinutes() << "\n"
		 << movie.getDescription() << "|"
		 << movie.getDirector() << "\n";

	file.close();
	return true;
}

bool saveShowToFile(const Show& show)
{
	std::ofstream file(Config::SHOWS_FILE, std::ios::app);
	if (!file) return false;

	file << show.showId << " "
		 << show.movieId << " "
		 << show.cinemaId << " "
		 << show.hallId << " "
		 << show.showTime << " "
		 << show.silverPrice << " "
		 << show.goldPrice << " "
		 << show.platinumPrice << " "
		 << show.isActive << "\n";

	file.close();
	return true;
}

}
