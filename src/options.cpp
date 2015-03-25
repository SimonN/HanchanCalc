#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "options.hpp"

Options::Options() {
  Exception = "";
  noValidArgFlag = true;
  newOptions();
}

Options::~Options() {
}

void Options::newOptions() {
  OptionSet newSet;
  newSet.helpFlag = false;
  newSet.showParamFlag = false;
  newSet.filename = "goe-stats.txt";
  newSet.from = "0001-01-01";
  newSet.before = "9999-12-31";
  newSet.reqHanchan = 3;
  newSet.pad = 8;
  newSet.decay = 1.0;
  newSet.sortParam = 1;
  useSet=options.size();
  options.push_back(newSet);
}

std::string Options::getBefore() {return options[useSet].before;}
double Options::getDecay() {return options[useSet].decay;}
std::string Options::getFilename() {return options[useSet].filename;}
std::string Options::getFrom() {return options[useSet].from;}
int Options::getPad() {return options[useSet].pad;}
int Options::getReqHanchan() {return options[useSet].reqHanchan;}
int Options::getSortParam() {return options[useSet].sortParam;}


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
      if(com=="-new") {
        invalidCommand = false;
        newOptions();
      }
      if(com=="-choose") {
        invalidCommand = false;
        i++;
        if(i<numArg && Arg[i][0]!='-') {
          if(Arg[i][0]>='0' && Arg[i][0]<='9') {
            noValidArgFlag = false;
            useSet = std::stoi(Arg[i])-1;
          } else {
            Exception += "Error in command \"-choose\":\n  ";
            Exception += "Parameter must start with a non-negative integer.";
            Exception += "\n";
          }
        } else {
          Exception += "Error: Missing command parameter in command \"-choose\".\n";
          i--;
        }
      }
      if(com=="-ignore") {
        invalidCommand = false;
        Exception = "";
        noValidArgFlag = false;
        options[useSet].helpFlag = false;
        options[useSet].showParamFlag = false;
        options[useSet].filename = "goe-stats.txt";
        options[useSet].from = "0001-01-01";
        options[useSet].before = "9999-12-31";
        options[useSet].reqHanchan = 3;
        options[useSet].pad = 8;
        options[useSet].decay = 1.0;
        options[useSet].sortParam = 1;
      }
      if(com=="-help") {
        invalidCommand = false;
        noValidArgFlag = false;
        options[useSet].helpFlag = true;
      }
      if(com=="-sp") {
        invalidCommand = false;
        noValidArgFlag = false;
        options[useSet].showParamFlag = true;
      }
      if(com=="-file") {
        invalidCommand = false;
        i++;
        if(i<numArg && Arg[i][0]!='-') {
          options[useSet].filename=Arg[i];
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
            options[useSet].from = param;
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
            options[useSet].before = param;
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
            options[useSet].from = param;
            options[useSet].before = Options::addDay(options[useSet].from);
            options[useSet].reqHanchan = 1;
            options[useSet].pad = 0;
            options[useSet].decay = 1.0;
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
        options[useSet].from = param;
        options[useSet].before = Options::addDay(options[useSet].from);
        options[useSet].reqHanchan = 1;
        options[useSet].pad = 0;
        options[useSet].decay = 1.0;
      }
      if(com=="-tonight") {
        invalidCommand = false;
        noValidArgFlag = false;
        std::string param = Options::makeDate("yesterday");
        options[useSet].from = param;
        options[useSet].before = Options::addDay(options[useSet].from);
        options[useSet].before = Options::addDay(options[useSet].before);
        options[useSet].reqHanchan = 1;
        options[useSet].pad = 0;
        options[useSet].decay = 1.0;
      }
      if(com=="-yesterday") {
        invalidCommand = false;
        noValidArgFlag = false;
        std::string param = Options::makeDate("yesterday");
        options[useSet].from = param;
        options[useSet].before = Options::addDay(options[useSet].from);
        options[useSet].reqHanchan = 1;
        options[useSet].pad = 0;
        options[useSet].decay = 1.0;
      }
      if(com=="-hcreq") {
        invalidCommand = false;
        i++;
        if(i<numArg && Arg[i][0]!='-') {
          if(Arg[i][0]>='0' && Arg[i][0]<='9') {
            noValidArgFlag = false;
            options[useSet].reqHanchan = std::stoi(Arg[i]);
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
            options[useSet].pad = std::stoi(Arg[i]);
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
            options[useSet].decay = std::stod(str);
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
          std::string Param=Arg[i];
          if(Param=="points" || Param=="R" || Param=="rank" || Param=="1st") {
            if(Param=="points") options[useSet].sortParam = 1;
            if(Param=="R") options[useSet].sortParam = 2;
            if(Param=="rank") options[useSet].sortParam = 3;
            if(Param=="1st") options[useSet].sortParam = 4;
          } else {
            if(Arg[i][0]>='0' && Arg[i][0]<='9') {
              noValidArgFlag = false;
              options[useSet].sortParam = std::stoi(Arg[i]);
            } else {
              Exception += "Error in command \"-sort\":\n  ";
              Exception += "Parameter must be \"points\", \"ELO\", \"rank\" or \"1st\".";
              Exception += "\n";
            }
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
  if(options[useSet].helpFlag) {
    std::cout << "Implemented commands are -ignore, -new, -choose,\n";
    std::cout << "  -help, -sp, -file, -from, -before,\n";
    std::cout << "  -date, -today, -tonight, -yesterday, -hcreq, -pad, -decay, -sort.\n";
    std::cout << "Also today and yesterday are valid dates.\n";
  }
}
void Options::showParam() {
  if(options[useSet].showParamFlag) {
    std::cout.setf(std::cout.left);
    std::cout << std::setw(8) << "file:" << options[useSet].filename << '\n';
    std::cout << std::setw(8) << "from:" << options[useSet].from << " (only count hanchans on or after this date)\n";
    std::cout << std::setw(8) << "before:" << options[useSet].before << " (only count hanchans strictly before this date)\n";
    std::cout << std::setw(8) << "hc-req:" << options[useSet].reqHanchan << " (players with less hanchans aren't listed)\n";
    std::cout << std::setw(8) << "pad:" << options[useSet].pad << " (if fewer HCs, pad averages with old HCs of 0 pt, pl 2.5)\n";
    std::cout << std::setw(8) << "decay:" << options[useSet].decay << " (weigh the n-th newest HC with decay^n for the average)\n";
    std::cout << std::setw(8) << "sort:" << options[useSet].sortParam << " (avg Points = 1; ELO = 2; avg Rank = 3; avg 1st = 4; name = 5)\n";
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
