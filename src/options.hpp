#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <string>

class Options {
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
    int reqHanchan;
    int pad;
    double decay;
    int sortParam;

    std::string makeDate(std::string date);
    std::string addDay(std::string date);
};

#endif /* OPTIONS_HPP */
