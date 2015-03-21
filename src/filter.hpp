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

  private:
    std::vector <Player> players;
};

#endif /* FILTER_HPP */
