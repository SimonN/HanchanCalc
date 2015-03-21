#include <iostream>
#include <cstdint>

#include "options.hpp"
#include "filter.hpp"



int main(int numArg, char ** Arg) {
  Options options;

  options.fileSet();
  options.set(numArg, Arg);

  options.showParam();

  Filter filter(options);
  filter.output_table();

  options.throwException();
  options.help();
  options.remHelp();
  return 0;
}


