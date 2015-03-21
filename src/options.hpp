#ifndef OPTIONS_H
#define OPTIONS_H

#include <ctime>
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <algorithm>
#include <fstream>

class Options
{
  public:
    Options();
    ~Options();

    std::string getFilename();

    void fileSet();
    void set(int numArg, char ** Arg);

    void remHelp();
    void help();
    void showParam();
    void throwException();


  private:
    std::string comException;
    std::string Exception;
    bool noValidArgFlag; //remember user to use -help
    bool helpFlag; //show help if available
    bool showParamFlag; //show parameters before output

    std::string filename;
    std::string from;
    std::string before;
    uint64_t reqHanchan;
    uint64_t pad;
    double decay;
    uint64_t sortParam;

    std::string makeDate(std::string date);
    std::string addDay(std::string date);
};

#endif /* OPTIONS_H */
