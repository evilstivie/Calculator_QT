#include "algo.h"
#include "hashmap.h"
#include "smath.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <cassert>

void test_main() {
  std::string s;
  std::getline(std::cin, s);
  
  HashMap<std::string, double, string_hash> vars;
  vars.put("e", EULER);
  vars.put("pi", PI);
  vars.put("x", 1.0);

  trim_unary(s, vars);  // replace unary plus and minus with 0 +- expr

  std::cout << "Pre-processed string = " << s << std::endl;

  std::string polish;
  to_polish(s, polish, vars);
  
  std::cout << "Polish notation = " << polish << std::endl;
  std::cout << "Result = " << std::fixed << std::setprecision(2) << calc_polish(polish) << std::endl;
}

void test_hashmap() {
  HashMap<std::string, double, string_hash> map;
  map.put("e", EULER);
  map.put("pi", PI);

  std::cout << map.get("e") << std::endl;
  std::cout << map.get("pi") << std::endl;
}

int main () {
  test_main();
  return 0;
}
