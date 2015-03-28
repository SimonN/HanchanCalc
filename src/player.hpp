#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

// hanchanPlayer is a single piece of information from a read hanchan.
// Each hanchan gives info about 4 hanchanPlayers. Each hanchanPlayer points
// to a Player playing many of these hanchans, via (int number).

struct hanchanPlayer {
  int number; // index of the corresponding player in the overall vector
  int points;
  bool operator < (const hanchanPlayer& a) const
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

enum PlayerSortBy {
    SORT_BY_NAME,
    SORT_BY_POINTS,
    SORT_BY_R,
    SORT_BY_PLACE,
    SORT_BY_TOPPU,
    SORT_MAX
};

struct PlayerSort {
  PlayerSort(PlayerSortBy val)
  {
    param = val;
  }
  bool operator () (const Player& a, const Player& b) const {
    if(param == SORT_BY_POINTS) {
      if(a.decayedTotal == 0) return false;
      if(b.decayedTotal == 0) return true;
      return a.decayedPoints / a.decayedTotal
           > b.decayedPoints / b.decayedTotal;
    }
    if(param == SORT_BY_R) return a.R > b.R;
    if(param == SORT_BY_PLACE) {
      if(a.decayedTotal == 0) return true;
      if(b.decayedTotal == 0) return false;
      return a.decayedPlaceSum / a.decayedTotal
           < b.decayedPlaceSum / b.decayedTotal;
    }
    if(param == SORT_BY_TOPPU) {
      if(a.decayedTotal == 0) return false;
      if(b.decayedTotal == 0) return true;
      return a.decayedToppu / a.decayedTotal
           > b.decayedToppu / b.decayedTotal;
    }
    return a.name < b.name;
  }
  PlayerSortBy param;
};

#endif /* PLAYER_HPP */
