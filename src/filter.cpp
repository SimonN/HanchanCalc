#include "filter.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

bool sorthp(hanchanPlayer i, hanchanPlayer j) {
  return true;
}


Filter::Filter(Options options) {
  std::ifstream input;
  std::string line;
  std::string param;
  hanchanPlayer player[4];
  int space;
  bool error;
  
  input.open(options.getFilename());
  while(std::getline(input, line)) {
    //check date
    param = line.substr(0,10);
    if(testDate(param)==false || line[10]!=' ') {
      Exception += "Error in line:\n ";
      Exception += line;
      Exception += "\n Date could not be read.\n";
      continue;
    }
    if(param<options.getFrom()) continue;
    if(param>=options.getBefore()) continue;
    
    param = line.substr(11);
    
    //get names and points
    error = false;
    for(int i=0; i<4; i++) {
      space = param.find(' ');
      while(space==0) {
        param = param.substr(1);
        space = line.find(' ');
      }
      player[i].name = param.substr(0,space);
      param = param.substr(space+1);
      space = param.find(' ');
      while(space==0) {
        param = param.substr(1);
        space = param.find(' ');
      }
      if(param[0]=='+') param = param.substr(1);
      if((param[0]>='0' && param[0]<='9') || (param[0]=='-' && param[1]>='0' && param[1]<='9')) {
        player[i].points = std::stoi(param.substr(0,space));
      } else {
        Exception += "Error in line:\n ";
        Exception += line;
        Exception += "\n Points should be an integer\n";
        error = 1;
        break;
      }
      param = param.substr(space+1);
    }
    if(error) continue;
    
    //check sum of points
    if(player[0].points+player[1].points+player[2].points+player[3].points!=0) {
      Exception += "Error in line:\n ";
      Exception += line;
      Exception += "\n Sum is not equal to 0.\n";
      continue;
    }
    
    //sort players
    std::sort(std::begin(player), std::end(player));

  }
  input.close();
}

Filter::~Filter() {

}

void Filter::output_table() {
  
}

void Filter::throwException() {
  if(Exception!="") {
    std::cerr << Exception;
  }
}

bool Filter::testDate(std::string Date) {
  if(Date[4]!='-' || Date[7]!='-') return false;
  if(Date[0]<'0' || Date[1]<'0' || Date[2]<'0' || Date[3]<'0') return false;
  if(Date[5]<'0' || Date[6]<'0' || Date[8]<'0' || Date[9]<'0') return false;
  if(Date[5]>'9' || Date[6]>'9' || Date[8]>'9' || Date[9]>'9') return false;
  return true;
}
