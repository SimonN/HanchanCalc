#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

struct hanchanPlayer {
  std::string name;
  int points;
  bool operator<(const hanchanPlayer& a) const
  {
      return points < a.points;
  }
};

class Player
{
  public:
    Player();
    ~Player();

  private:
    
};

#endif /* PLAYER_HPP */
