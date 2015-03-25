#include "player.hpp"

Player::Player(std::string passedName) {
  name = passedName;
  totalHanchans = 0;
  decayedTotal = 0;
  decayedPoints = 0;
  decayedPlaceSum = 0;
  decayedToppu = 0;
  decayedRentai = 0;
  decayedRasu = 0;
  R = 1500;
}

double Player::avgPoints() {
  if(decayedTotal==0) return 0;
  return decayedPoints/decayedTotal;
}

double Player::avgPlace() {
  if(decayedTotal==0) return 0;
  return decayedPlaceSum/decayedTotal;
}

double Player::avgFirst() {
  if(decayedTotal==0) return 0;
  return decayedToppu/decayedTotal*100;
}

double Player::avgFirstH() {
  if(decayedTotal==0) return 0;
  return decayedRentai/decayedTotal*100;
}

double Player::avgLast() {
  if(decayedTotal==0) return 0;
  return decayedRasu/decayedTotal*100;
}
