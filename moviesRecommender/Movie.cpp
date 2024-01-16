
#include "Movie.h"
#define HASH_START 17
#define RES_MULT 31

/**
 * hash function used for a unordered_map (implemented for you)
 * @param movie shared pointer to movie
 * @return an integer for the hash map
 */
std::size_t sp_movie_hash(const sp_movie& movie){
    std::size_t res = HASH_START;
    res = res * RES_MULT + std::hash<std::string>()(movie->get_name());
    res = res * RES_MULT + std::hash<int>()(movie->get_year());
    return res;
}

/**
 * equal function used for an unordered_map (implemented for you)
 * @param m1
 * @param m2
 * @return true if the year and name are equal else false
 */
bool sp_movie_equal(const sp_movie& m1,const sp_movie& m2){
    return !(*m1 < *m2) && !(*m2 < *m1);
}


Movie::Movie(const std::string& name, int year): _exit_year(year)
{
    _name = name;
}

const std::string& Movie::get_name() const
{
    return this->_name;
}

int Movie::get_year() const
{
    return this->_exit_year;
}

bool Movie::operator< (Movie& other) const
{
    if (this->_exit_year != other._exit_year)
    { // if the movie's exit year isn't the same
        return this->_exit_year<other._exit_year;
    }
    return this->_name<other._name;
}

std::ostream& operator<< (std::ostream& os, const Movie& m)
{
    return os<<m._name<<" ("<<m._exit_year<<")"<<"\n";
}