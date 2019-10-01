#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include "algo.h"
#include "hashmap.h"

void test_main() {
  std::string s;
  std::getline(std::cin, s);

  std::cout << std::fixed << std::setprecision(6);

  hashmap<std::string, double, string_hash> vars;
  hashmap<std::string, double, string_hash> empty;
  vars.put("e", EULER);
  vars.put("pi", PI);
  empty.put("e", EULER);
  empty.put("pi", PI);

  std::string inp;
  while (std::cin >> inp) {
    if (inp == "exit")
      break;
    std::string value;
    std::cin >> value;
	trim_unary(value, empty);

	std::string polish;
	to_polish(value, polish, empty);

    double d = calc_polish(polish);
    vars.put(inp, d);
  }

  trim_unary(s, vars);  // replace unary plus and minus with 0 +- expr
  std::cout << "Pre-processed string = " << s << std::endl;

  std::string polish;
  to_polish(s, polish, vars);

  std::cout << "Polish notation = " << polish << std::endl;
  std::cout << "Result = " << calc_polish(polish) << std::endl;
}

int main() {
  test_main();
  #ifdef _WIN32
	system("pause");
  #endif
  return 0;
}
