#pragma once
#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include <vector>

namespace MovieManager {
    std::vector<std::string> listMovies();
    bool addMovie(const std::string& title);
    bool addMovieWithTime(const std::string& title, const std::string& showTime);
}

#endif // MOVIE_H
