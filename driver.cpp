#include "algo.h"
#include "smath.h"
#include <iostream>
#include <string>
#include <iomanip>

int main () {
  std::string s;
  std::getline(std::cin, s);
  std::string polish;
  to_polish(s, polish);
  std::cout << polish << std::endl;
  std::cout << "Eval = " << std::fixed << std::setprecision(15) << calc_polish(polish) << std::endl;
  return 0;
}
