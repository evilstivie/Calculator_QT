#include "algo.h"
#include "hashmap.h"
#include <cmath>
#include <iostream>
#include <string>
#include <iomanip>
#include <cassert>

void test_main() {
  std::string s;
  std::getline(std::cin, s);

  std::cout <<  std::fixed << std::setprecision(4);
  
  HashMap<std::string, double, string_hash> vars;
  vars.put("e", EULER);
  vars.put("pi", PI);

  std::string inp;
  while (std::cin >> inp) {
    std::cout << "name " << inp << std::endl;
    if (inp == "exit")
      break;
    std::string value;
    std::cin >> value;
    std::cout << " value to parse " << value << std::endl;
    int _ = 0;
    double d = read_double(value, _);
    vars.put(inp, d); 
    std::cout << "got it: " << inp << " => " << d << std::endl;
  }

  trim_unary(s, vars);  // replace unary plus and minus with 0 +- expr

  std::cout << "Pre-processed string = " << s << std::endl;

  std::string polish;
  to_polish(s, polish, vars);
  
  std::cout << "Polish notation = " << polish << std::endl;
  std::cout << "Result = " << calc_polish(polish) << std::endl;
}

int main () {
  test_main();
  return 0;
}
