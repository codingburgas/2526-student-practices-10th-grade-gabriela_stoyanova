#include "Movie.h"
#include "Config.h"
#include <fstream>

namespace MovieManager {

std::vector<std::string> listMovies()
{
    std::vector<std::string> movies;
    std::ifstream in(Config::MOVIES_FILE);
    if (!in) return movies;
    std::string line;
    while (std::getline(in, line))
    {
        if (!line.empty())
        {
            size_t pos = line.find('|');
            if (pos != std::string::npos)
                movies.push_back(line.substr(0, pos));
            else
                movies.push_back(line);
        }
    }
    return movies;
}

bool addMovie(const std::string& title)
{
    return addMovieWithTime(title, "");
}

bool addMovieWithTime(const std::string& title, const std::string& showTime)
{
    std::ofstream out(Config::MOVIES_FILE, std::ios::app);
    if (!out) return false;
    if (!showTime.empty())
        out << title << "|" << showTime << "\n";
    else
        out << title << "\n";
    return true;
}

} // namespace MovieManager
