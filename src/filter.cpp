#include "filter.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

Filter::Filter(Options options) {
  std::ifstream input;
  std::string line;
  hanchanPlayer currentPlayer[4];

  nameLength = 0;

  input.open(options.getFilename());
  while(std::getline(input, line)) {
    line += ' ';

    if(checkDate(options, line)==false) continue;

    if(getHanchan(currentPlayer, line)==false) continue;

    std::sort(&currentPlayer[0], &currentPlayer[4]);

    rcalcHanchan(currentPlayer);
    calcHanchan(options, currentPlayer);


  }
  input.close();
  addPadding(options);
}

Filter::~Filter() {

}

void Filter::output_table(Options options) {
  PlayerSortBy sortParam = options.getSortParam();
  int reqHanchan = options.getReqHanchan();;
  std::sort(&players[0], &players[players.size()], PlayerSort(sortParam));
  std::cout << std::left << std::setw(nameLength) << "Name" << " | " ;
  std::cout << std::right << std::setw(4) << "RVal" << " | " ;
  std::cout << std::right << std::setw(6) << "avPts" << " | " ;
  std::cout << std::right << std::setw(5) << "avPlc" << " | " ;
  std::cout << std::right << std::setw(4) << "1st" << " | " ;
  std::cout << std::right << std::setw(4) << "1+2" << " | " ;
  std::cout << std::right << std::setw(4) << "4th" << " | " ;
  std::cout << std::right << std::setw(4) << "HC" << " | " ;
  std::cout << '\n';
  std::cout << std::setfill('-') << std::setw(nameLength+54) << '-' << '\n';
  std::cout << std::setfill(' ');
  std::cout.setf(std::ios::fixed, std::ios::floatfield);
  std::cout.precision(2);
  for(size_t i=0; i<players.size(); i++) {
    if(players[i].totalHanchans < reqHanchan) continue;
    std::cout << std::left << std::setw(nameLength) << players[i].name << " | " ;
    std::cout.precision(0);
    std::cout << std::right << std::setw(4) << players[i].R << " | " ;
    std::cout.precision(2);
    std::cout << std::right << std::setw(6) << players[i].avgPoints() << " | ";
    std::cout.precision(3);
    std::cout << std::right << std::setw(5) << players[i].avgPlace() << " | ";
    std::cout.precision(0);
    std::cout << std::right << std::setw(3) << players[i].avgFirst() << "%" << " | ";
    std::cout.precision(0);
    std::cout << std::right << std::setw(3) << players[i].avgFirstH() << "%" << " | ";
    std::cout.precision(0);
    std::cout << std::right << std::setw(3) << players[i].avgLast() << "%" << " | ";
    std::cout.precision(0);
    std::cout << std::right << std::setw(4) << players[i].totalHanchans << " | ";
    std::cout << '\n';
  }
}

void Filter::throwException() {
  if(Exception!="") {
    std::cerr << Exception;
  }
}

bool Filter::checkDate(Options options, std::string line) {
  std::string date;
  bool error = false;

  if(line[10]!=' ') error = true;
  if(line[4]!='-' || line[7]!='-') error = true;
  if(line[0]<'0' || line[1]<'0' || line[2]<'0' || line[3]<'0') error = true;
  if(line[5]<'0' || line[6]<'0' || line[8]<'0' || line[9]<'0') error = true;
  if(line[5]>'9' || line[6]>'9' || line[8]>'9' || line[9]>'9') error = true;

  if(error) {
    Exception += "Error in line:\n ";
    Exception += line;
    Exception += "\n Date could not be read.\n";
    return false;
  }

  date = line.substr(0,10);
  if(date<options.getFrom()) return false;
  if(date>=options.getBefore()) return false;

  return true;
}

bool Filter::getHanchan(hanchanPlayer player[], std::string line) {
  std::string data=line.substr(11);
  std::string name;
  int space;
  //get names and points
  for(int i=0; i<4; i++) {
    space = data.find(' ');
    while(space==0) {
      data = data.substr(1);
      space = line.find(' ');
    }
    name = data.substr(0,space);
    if(playerNames.count(name) == 0) addPlayer(name);
    player[i].number = playerNames.at(name);
    data = data.substr(space+1);
    space = data.find(' ');
    while(space==0) {
      data = data.substr(1);
      space = data.find(' ');
    }
    if(data[0]=='+') {
      data = data.substr(1);
      space--;
    }
    if((data[0]>='0' && data[0]<='9') || (data[0]=='-' && data[1]>='0' && data[1]<='9')) {
      player[i].points = std::stoi(data.substr(0,space));
    } else {
      Exception += "Error in line:\n ";
      Exception += line;
      Exception += "\n Points should be an integer\n";
      return false;
    }
    data = data.substr(space+1);
  }

  //check sum of points
  if(player[0].points+player[1].points+player[2].points+player[3].points!=0) {
    Exception += "Error in line:\n ";
    Exception += line;
    Exception += "\n Sum is not equal to 0.\n";
    return false;
  }
  return true;
}

void Filter::addPlayer(std::string name) {
  Player newPlayer(name);
  playerNames.insert(std::pair<std::string,int>(name,players.size()));
  players.push_back(newPlayer);
  if(nameLength<name.length()) nameLength=name.length();
}


void Filter::calcHanchan(Options options, hanchanPlayer player[])
{
  const double decay = options.getDecay();

  for (int i = 0; i < 4; i++) {
    Player& p = players[player[i].number];
    p.totalHanchans++;
    p.decayedTotal    = (p.decayedTotal    * decay) + 1;
    p.decayedPoints   = (p.decayedPoints   * decay) + player[i].points;
    p.decayedPlaceSum = (p.decayedPlaceSum * decay) + (4-i);
  }
  Player& winner = players[player[3].number];
  Player& second = players[player[2].number];
  Player& fourth = players[player[0].number];

  winner.decayedToppu  = (winner.decayedToppu  * decay) + 1;
  winner.decayedRentai = (winner.decayedRentai * decay) + 1;
  second.decayedRentai = (second.decayedRentai * decay) + 1;
  fourth.decayedRasu   = (fourth.decayedRasu   * decay) + 1;
}

void Filter::rcalcHanchan(hanchanPlayer player[]) {
  double old[4];
  double s=0;
  double k = 20;
  for(int i=0; i<4; i++) {
    old[i] = players[player[i].number].R;

  }
  for(int i=0; i<4; i++) {
    for(int j=0; j<4; j++) {
      if(i<j) s=0;
      if(i==j) s=0.5;
      if(i>j) s=1;
      k = 15-0.1*players[player[i].number].totalHanchans;
      if(k<3) k = 3;
      players[player[i].number].R += k*(s-(1/(1+pow(10, (old[j]-old[i])/400))));
    }
  }
}

void Filter::addPadding(Options options) {
  int padding = options.getPad();
  double decay = options.getDecay();
  double cumDecay;
  for(size_t i=0; i<players.size(); i++) {
    cumDecay = 1;
    for(int j=0; j<players[i].totalHanchans; j++) cumDecay *= decay;
    for(int j=players[i].totalHanchans; j<padding; j++) {
      players[i].decayedTotal += cumDecay;
      players[i].decayedPlaceSum += 2.5*cumDecay;
      players[i].decayedToppu += 0.25*cumDecay;
      players[i].decayedRentai += 0.5*cumDecay;
      players[i].decayedRasu += 0.25*cumDecay;
      cumDecay *= decay;
    }
  }
}
