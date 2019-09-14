#include "algo.h"
#include "smath.h"
#include <iostream>
#include <string>
#include <iomanip>

int main () {
  std::string s;
  std::getline(std::cin, s);

  std::string lol;
  for (int i = 0; i < s.size(); i++) {
    char c = s[i];
    bool prev = i == 0 ? false : (isdigit(s[i - 1]) || s[i - 1] == ')');
    bool next = i == s.size() - 1 ? false : (isdigit(s[i + 1]) || s[i + 1] == ')');
    if (!prev && next && (c == '+' || c == '-')) {
      ++i;
      double d = read_double(s, i);
      --i;
      if (c == '+')
        lol += std::to_string(d) + "+0";
      if (c == '-')
        lol += "0-" + std::to_string(d);
    } else 
      lol += c;
  }

  std::cout << "Pre-processed string = " << lol << std::endl;

  std::string polish;
  to_polish(lol, polish);
  
  std::cout << "Polish notation = " << polish << std::endl;
  std::cout << "Eval = " << std::fixed << std::setprecision(2) << calc_polish(polish) << std::endl;
  return 0;
}
