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

  std::cout << std::fixed << std::setprecision(4);

  HashMap<std::string, double, string_hash> vars;
  vars.put("e", EULER);
  vars.put("pi", PI);

  std::string inp;
  while (std::cin >> inp) {
    if (inp == "exit")
      break;
    std::string value;
    std::cin >> value;

    int _ = 0;
    double d = read_double(value, _);

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
