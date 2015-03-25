#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

struct hanchanPlayer {
  int number;
  int points;
  bool operator<(const hanchanPlayer& a) const
  {
      return points < a.points;
  }
};

struct Player {
  Player(std::string name);
  int totalHanchans;
  double decayedTotal;
  double decayedPoints;
  double decayedPlaceSum;
  double decayedToppu;
  double decayedRentai;
  double decayedRasu;
  double R;
  
  double avgPoints();
  double avgPlace();
  double avgFirst();
  double avgFirstH();
  double avgLast();
  
  std::string name;
};

struct PlayerSort{
  PlayerSort(int val) {
    param = val;
  }
  bool operator () (Player a, Player b) {
    if(param == 1) {
      if(a.decayedTotal == 0) return false;
      if(b.decayedTotal == 0) return true;
      return a.decayedPoints/ a.decayedTotal > b.decayedPoints/ b.decayedTotal;
    }
    if(param == 2) return a.R > b.R;
    if(param == 3) {
      if(a.decayedTotal == 0) return true;
      if(b.decayedTotal == 0) return false;
      return a.decayedPlaceSum/ a.decayedTotal < b.decayedPlaceSum/ b.decayedTotal;
    }
    if(param == 4) {
      if(a.decayedTotal == 0) return false;
      if(b.decayedTotal == 0) return true;
      return a.decayedToppu/ a.decayedTotal > b.decayedToppu/ b.decayedTotal;
    }
    return a.name < b.name;
  }
  int param;
};


#endif /* PLAYER_HPP */
