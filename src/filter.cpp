#include "filter.hpp"

#include <fstream>
#include <iostream>

Filter::Filter(Options options) {
  std::ifstream input;
  input.open(options.getFilename());





  input.close();
}

Filter::~Filter() {

}

void Filter::output_table() {
  std::cout << "filter\n";
}
