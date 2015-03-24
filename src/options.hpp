#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <string>
#include <vector>

struct OptionSet {
  bool helpFlag; //show help if available
  bool showParamFlag; //show parameters before output
  std::string filename;
  std::string from;
  std::string before;
  int reqHanchan;
  int pad;
  double decay;
  int sortParam;
};

class Options {
  public:
    Options();
    ~Options();
    
    void newOptions();

    std::string getFilename();
    std::string getFrom();
    std::string getBefore();

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

    std::vector <OptionSet> options;
    int useSet;

    std::string makeDate(std::string date);
    std::string addDay(std::string date);
};

#endif /* OPTIONS_HPP */
