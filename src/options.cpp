#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "options.hpp"

Options::Options() {
  Exception = "";
  noValidArgFlag = true;
  helpFlag = false;
  showParamFlag = false;
  filename = "goe-stats.txt";
  from = "0001-01-01";
  before = "9999-12-31";
  reqHanchan = 3;
  pad = 8;
  decay = 1.0;
  sortParam = 1;
}

Options::~Options() {
}

std::string Options::getFilename() {
  return filename;
}

void Options::fileSet() {
  std::ifstream config;
  std::string line;
  config.open("config.txt");
  while(std::getline(config, line)) {
    line += " ";
    if(line[0]!='/') {
      std::vector <std::string> arg;
      int space;
      space = line.find(' ');
      while(space!=-1) {
        if(space!=0) {
          arg.push_back(line.substr(0,space));
        }
        line = line.substr(space+1);
        space = line.find(' ');
      }
      char * v[arg.size()+1];
      for(size_t i=0; i<arg.size(); i++) {
        v[i+1]=&(arg[i])[0];
      }
      Options::set(arg.size()+1, v);
  }
  }
  config.close();

}

void Options::set(int numArg, char ** Arg) {
  for(int i=1; i<numArg; i++) {
    if(Arg[i][0]=='-') {
      std::string com = Arg[i];
      bool invalidCommand = true;
      if(com=="-ignore") {
        invalidCommand = false;
        Exception = "";
        noValidArgFlag = false;
        helpFlag = false;
        showParamFlag = false;
        filename = "goe-stats.txt";
        from = "0001-01-01";
        before = "9999-12-31";
        reqHanchan = 3;
        pad = 8;
        decay = 1.0;
        sortParam = 1;
      }
      if(com=="-help") {
        invalidCommand = false;
        noValidArgFlag = false;
        helpFlag = true;
      }
      if(com=="-sp") {
        invalidCommand = false;
        noValidArgFlag = false;
        showParamFlag = true;
      }
      if(com=="-file") {
        invalidCommand = false;
        i++;
        if(i<numArg && Arg[i][0]!='-') {
          filename=Arg[i];
        } else {
          Exception += "Error: Missing command parameter in command \"-file\".\n";
          i--;
        }
      }
      if(com=="-from") {
        invalidCommand = false;
        i++;
        if(i<numArg && Arg[i][0]!='-') {
          std::string param = Options::makeDate(Arg[i]);
          if(param!="") {
            from = param;
            noValidArgFlag = false;
          } else {
            Exception += "Error in command \"-from\":\n  ";
            Exception += comException;
            Exception += "\n";
          }
        } else {
          Exception += "Error: Missing command parameter in command \"-from\".\n";
          i--;
        }
      }
      if(com=="-before") {
        invalidCommand = false;
        i++;
        if(i<numArg && Arg[i][0]!='-') {
          std::string param = Options::makeDate(Arg[i]);
          if(param!="") {
            before = param;
            noValidArgFlag = false;
          } else {
            Exception += "Error in command \"-before\":\n  ";
            Exception += comException;
            Exception += "\n";
          }
        } else {
          Exception += "Error: Missing command parameter in command \"-before\".\n";
          i--;
        }
      }
      if(com=="-date") {
        invalidCommand = false;
        i++;
        if(i<numArg && Arg[i][0]!='-') {
          std::string param = Options::makeDate(Arg[i]);
          if(param!="") {
            noValidArgFlag = false;
            from = param;
            before = Options::addDay(from);
            reqHanchan = 1;
            pad = 0;
            decay = 1.0;
          } else {
            Exception += "Error in command \"-date\":\n  ";
            Exception += comException;
            Exception += "\n";
          }
        } else {
          Exception += "Error: Missing command parameter in command \"-date\".\n";
          i--;
        }
      }
      if(com=="-today") {
        invalidCommand = false;
        noValidArgFlag = false;
        std::string param = Options::makeDate("today");
        from = param;
        before = Options::addDay(from);
        reqHanchan = 1;
        pad = 0;
        decay = 1.0;
      }
      if(com=="-tonight") {
        invalidCommand = false;
        noValidArgFlag = false;
        std::string param = Options::makeDate("yesterday");
        from = param;
        before = Options::addDay(from);
        before = Options::addDay(before);
        reqHanchan = 1;
        pad = 0;
        decay = 1.0;
      }
      if(com=="-yesterday") {
        invalidCommand = false;
        noValidArgFlag = false;
        std::string param = Options::makeDate("yesterday");
        from = param;
        before = Options::addDay(from);
        reqHanchan = 1;
        pad = 0;
        decay = 1.0;
      }
      if(com=="-hcreq") {
        invalidCommand = false;
        i++;
        if(i<numArg && Arg[i][0]!='-') {
          if(Arg[i][0]>='0' && Arg[i][0]<='9') {
            noValidArgFlag = false;
            reqHanchan = std::stol(Arg[i]);
          } else {
            Exception += "Error in command \"-hcreq\":\n  ";
            Exception += "Parameter must start with a non-negative integer.";
            Exception += "\n";
          }
        } else {
          Exception += "Error: Missing command parameter in command \"-hcreq\".\n";
          i--;
        }
      }
      if(com=="-pad") {
        invalidCommand = false;
        i++;
        if(i<numArg && Arg[i][0]!='-') {
          if(Arg[i][0]>='0' && Arg[i][0]<='9') {
            noValidArgFlag = false;
            pad = std::stol(Arg[i]);
          } else {
            Exception += "Error in command \"-pad\":\n  ";
            Exception += "Parameter must start with a non-negative integer.";
            Exception += "\n";
          }
        } else {
          Exception += "Error: Missing command parameter in command \"-pad\".\n";
          i--;
        }
      }
      if(com=="-decay") {
        invalidCommand = false;
        i++;
        if(i<numArg && Arg[i][0]!='-') {
          if(Arg[i][0]>='0' && Arg[i][0]<='9') {
            std::string str(Arg[i]);
            int pos = str.find(',');
            if(pos!=-1) {
              str[pos]='.';
            }
            noValidArgFlag = false;
            decay = std::stod(str);
          } else {
            Exception += "Error in command \"-decay\":\n  ";
            Exception += "Parameter must start with a non-negative rational number.";
            Exception += "\n";
          }
        } else {
          Exception += "Error: Missing command parameter in command \"-decay\".\n";
          i--;
        }
      }
      if(com=="-sort") {
        invalidCommand = false;
        i++;
        if(i<numArg && Arg[i][0]!='-') {
          if(Arg[i][0]>='0' && Arg[i][0]<='9') {
            noValidArgFlag = false;
            sortParam = std::stol(Arg[i]);
          } else {
            Exception += "Error in command \"-sort\":\n  ";
            Exception += "Parameter must start with a non-negative integer.";
            Exception += "\n";
          }
        } else {
          Exception += "Error: Missing command parameter in command \"-sort\".\n";
          i--;
        }
      }
      if(invalidCommand) {
        Exception += "Error: Command \"";
        Exception += Arg[i];
        Exception += "\" not implemented (yet).\n";
      }
    } else {
      Exception += "Error: Missing command for parameter \"";
      Exception += Arg[i];
      Exception += "\".\n";
    }
  }
}

void Options::remHelp() {
  if(noValidArgFlag) {
    std::cout << "If the result is not as expected try \"-help\".\n";
  }
}
void Options::help() {
  if(helpFlag) {
    std::cout << "Implemented commands are -ignore, -help, -sp, -file, -from, -before,\n";
    std::cout << "  -date, -today, -tonight, -yesterday, -hcreq, -pad, -decay, -sort.\n";
    std::cout << "Also today and yesterday are valid dates.\n";
  }
}
void Options::showParam() {
  if(showParamFlag) {
    std::cout.setf(std::cout.left);
    std::cout << std::setw(8) << "file:" << filename << '\n';
    std::cout << std::setw(8) << "from:" << from << " (only count hanchans on or after this date)\n";
    std::cout << std::setw(8) << "before:" << before << " (only count hanchans strictly before this date)\n";
    std::cout << std::setw(8) << "hc-req:" << reqHanchan << " (players with less hanchans aren't listed)\n";
    std::cout << std::setw(8) << "pad:" << pad << " (if fewer HCs, pad averages with old HCs of 0 pt, pl 2.5)\n";
    std::cout << std::setw(8) << "decay:" << decay << " (weigh the n-th newest HC with decay^n for the average)\n";
    std::cout << std::setw(8) << "sort:" << sortParam << " (Simon = 1; ELO = 2; avg Rank = 3; avg 1st = 4)\n";
  }
}
void Options::throwException() {
  if(Exception!="") {
    std::cerr << Exception;
  }
}


std::string Options::makeDate(std::string dateraw) {
  if(dateraw=="today") {
    char buffer[12];
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (buffer,11,"%Y-%m-%d",timeinfo);
    std::string str(buffer);
    return str;
  }
  if(dateraw=="yesterday") {
    char buffer[12];
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    rawtime -= 24*60*60;
    timeinfo = localtime (&rawtime);
    strftime (buffer,11,"%Y-%m-%d",timeinfo);
    std::string str(buffer);
    return str;
  }
  std::string year, month, day, date;
  int t;
  t = dateraw.find('-');
  if(t==-1) {
    t=dateraw.length();
    dateraw = dateraw+'-';
  }
  if(t>4) {
    comException = "\"Year\" is not allowed to have more than 4 characters.";
    return "";
  }
  year = dateraw.substr(0,t);
  while(year.length()<4) year = "0"+year;
  for(int i=0; i<4; i++) {
    if(year[i]<'0' || year[i]>'9') {
      comException = "\"Year\" should only have digits.";
      return "";
    }
  }
  dateraw = dateraw.substr(t+1);
  t = dateraw.find('-');
  if(t==-1) {
    t=dateraw.length();
    dateraw = dateraw+'-';
  }
  if(t>2) {
    comException = "\"Month\" is not allowed to have more than 2 characters.";
    return "";
  }
  month = dateraw.substr(0,t);
  while(month.length()<2) month = "0"+month;
  for(int i=0; i<2; i++) {
    if(month[i]<'0' || month[i]>'9') {
      comException = "\"Month\" should only have digits.";
      return "";
    }
  }
  if(month=="00") month="01";
  if(month>"12") {
    comException = "\"Month\" is not allowed to be >12.";
    return "";
  }
  dateraw = dateraw.substr(t+1);
  t = dateraw.find('-');
  if(t==-1) {
    t=dateraw.length();
  }
  if(t>2) {
    comException = "\"Day\" is not allowed to have more than 2 characters.";
    return "";
  }
  day = dateraw.substr(0,t);
  while(day.length()<2) day = "0"+day;
  for(int i=0; i<2; i++) {
    if(day[i]<'0' || day[i]>'9') {
      comException = "\"Day\" should only have digits.";
      return "";
    }
  }
  if(day=="00") day="01";
  if(day>"31") {
    comException = "\"Day\" is not allowed to be >31.";
    return "";
  }

  date = year+'-'+month+'-'+day;

  return date;
}

std::string Options::addDay(std::string dateraw) {
  if(dateraw[9]!='9') {
    dateraw[9]+=1;
    return dateraw;
  }
  dateraw[9]='0';
  dateraw[8]+=1;
  return dateraw;
}
