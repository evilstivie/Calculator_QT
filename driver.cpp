#include "algo.h"
#include <iostream>

int main () {
  std::string inp;
  std::getline(std::cin, inp);
  std::cout << calc_polish(inp) << std::endl;
  return 0;
}
