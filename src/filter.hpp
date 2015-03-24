#ifndef FILTER_HPP
#define FILTER_HPP

#include <vector>

#include "options.hpp"
#include "player.hpp"

class Filter
{
  public:
    Filter(Options options);
    ~Filter();

    void output_table();
    
    void throwException();

  private:
    std::string Exception;
    std::vector <Player> players;
    int nameLength;
    
    bool testDate(std::string Date);
};

#endif /* FILTER_HPP */
