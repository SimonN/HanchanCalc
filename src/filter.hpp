#ifndef FILTER_H
#define FILTER_H

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

#endif /* FILTER_ */
