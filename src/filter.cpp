#include "filter.hpp"

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
