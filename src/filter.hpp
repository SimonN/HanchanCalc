#ifndef FILTER_HPP
#define FILTER_HPP

#include <map>
#include <vector>

#include "options.hpp"
#include "player.hpp"

class Filter
{
  public:
    Filter(Options options);
    ~Filter();

    void output_table(Options options);
    
    void throwException();

  private:
    std::string Exception;
    std::vector <Player> players;
    std::map <std::string,int> playerNames;
    size_t nameLength;
    
    bool checkDate(Options options, std::string line);
    bool getHanchan(hanchanPlayer player[], std::string line);
    void calcHanchan(Options options, hanchanPlayer player[]);
    void rcalcHanchan(hanchanPlayer player[]);
    
    void addPlayer(std::string name);
    void addPadding(Options options);
};


#endif /* FILTER_HPP */
