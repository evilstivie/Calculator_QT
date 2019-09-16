#ifndef _CALCALGO_H
#define _CALCALGO_H
#define _USE_MATH_DEFINES

#include <string>
#include "hashmap.h"
#include "math.h"
#include "stack.h"

const double EPS = 1e-8;
const double PI = 3.14159265358979323846;
const double EULER = 2.7182818284590452353602874713527;
const double INF = 1e20;

const char OPERATIONS[5] = {'+', '-', '*', '/', '^'};

bool isdigit(char c) {
  return '0' <= c && c <= '9';
}

bool iskey(char cur) {
  for (char c : OPERATIONS)
    if (c == cur)
      return true;
  return false;
}

bool isspace(char c) {
  return c == ' ';
}

bool isname(char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

bool opening(char c) {
  return c == '(' || c == '[';
}

bool closing(char c) {
  return c == ')' || c == ']';
}

int get_prior(char op) {
  switch (op) {
    case '+':
    case '-':
      return 1;
    case '*':
    case '/':
      return 2;
    default:
      return -1;
  }
}

double read_single(const std::string& str, int& pos) {
  double d = 0;
  char c = str[pos];
  int start = pos;

  bool sign = false;
  if (c == '-' || c == '+') {
    ++pos;
    sign = c == '-';
    c = str[pos];
  }

  int prev = pos;
  while (pos < str.size() && isdigit(c)) {
    d = d * 10.0 + (c - '0');
    c = str[++pos];
  }
  int post = pos;
  if ((str[start] == '-' || str[start] == '+') && post == prev) {
    pos = start;
    return INF;
  }

  return (sign ? -1.0 : 1.0) * d;
}

double read_double(const std::string& str, int& pos) {
  int prev = pos;
  double d = read_single(str, pos);
  if (pos >= str.size()) {
    return d;
  }
  int now = pos;

  if (now == prev) {
    pos = 0;
    return INF;
  }

  if (str[pos] == 'e' || str[pos] == 'E') {
    ++pos;
    int prev = pos;
    double man = read_single(str, pos);

    int now = pos;
    if (now - prev == 0) {
      pos = 0;
      return INF;
    }

    return d * pow(10, man);
  } else if (str[pos] != '.') {
    return d;
  }

  ++pos;
  if (pos >= str.size()) {
    pos = 0;
    return INF;
  }

  int pprev = pos;
  char c = str[pos];
  double ten = 10.0;
  while (pos < str.size() && isdigit(c)) {
    if (d < 0)
      d -= double(c - '0') / ten;
    else
      d += double(c - '0') / ten;
    ten *= 10;
    c = str[++pos];
  }
  std::cout << std::endl;

  int nnow = pos;
  if (pprev == nnow) {
    pos = 0;
    return INF;
  }

  if (pos < str.size() - 1 && (str[pos] == 'e' || str[pos] == 'E')) {
    ++pos;
    double v = read_single(str, pos);
    return d * pow(10, v);
  }

  return d;
}

std::string get_word(const std::string& s, int& pos) {
  if (isdigit(s[pos]))
    return "";
  if (pos != 0 && isdigit(s[pos - 1]) && (s[pos] == 'e' || s[pos] == 'E'))
    return "";

  std::string res;
  while (pos < s.size()) {
    char c = s[pos];
    if (!isname(c) && !isdigit(c))
      break;
    res += c;
    pos++;
  }

  return res;
}

void trim_unary(std::string& s,
                HashMap<std::string, double, string_hash>& vars) {
  std::string first;
  for (int i = 0; i < s.size(); i++) {
    int before = i;
    std::string name = get_word(s, i);
    std::cout << "variable " << name << std::endl;
    if (name == "cos") {
      --i;
      first += name;
      continue;
    }
    if (i != before) {
      std::cout << "lol " << std::to_string(vars.get(name)) << std::endl;
      first += std::to_string(vars.get(name));
      --i;
    } else {
      first += s[i];
      std::cout << "adding raw " << s[i] << std::endl;
    }
  }
  s = first;
  s = "(" + s + ")";
  std::string ans;
  ans += '(';
  for (int i = 1; i < s.size(); i++) {
    char c = s[i];
    if (isspace(c))
      continue;

    if ((opening(s[i - 1]) || iskey(s[i - 1])) && (c == '+' || c == '-')) {
      ans += "0";
      ans += c;
    } else
      ans += c;
  }

  s = ans;
}

void to_polish(const std::string& inp,
               std::string& outp,
               HashMap<std::string, double, string_hash>& vars) {
  int intro = 0;
  stack<char> st;

  do {
    char c = inp[intro];
    switch (c) {
      case '+':
      case '-':
        while (!st.empty()) {
          char op = st.top();
          if (!opening(op)) {
            outp = outp + op;
            st.pop();
          } else
            break;
        }
        st.push(c);
        break;

      case '*':
      case '/':
        while (!st.empty()) {
          char op = st.top();
          if (op == '^' || op == '*' || op == '/') {
            outp = outp + op;
            st.pop();
          } else
            break;
        }
        st.push(c);
        break;

      case '(':
      case '[':
        st.push(c);
        break;

      case ')':
        while (!st.empty()) {
          char op = st.top();
          st.pop();
          if (op == '(')
            break;
          else
            outp += op;
        }
        break;

      case ']':
        while (!st.empty()) {
          char op = st.top();
          st.pop();
          if (op == '[')
            break;
          else
            outp = outp + op;
        }
        break;

      case '^':
        st.push(c);
        break;

      default:
        std::string str;
        if (isdigit(c)) {
          // number
          str = std::to_string(read_double(inp, intro));
        } else {
          // variable
          std::string name = get_word(inp, intro);
          str = std::to_string(vars.get(name));
        }

        --intro;
        outp = outp + str + ' ';
        break;
    }

    intro++;

  } while (intro < inp.size());

  while (!st.empty()) {
    outp = outp + st.top();
    st.pop();
  }
}

double calc_polish(const std::string& post) {
  stack<double> st;
  double num = 0;

  for (int i = 0; i < post.size(); i++) {
    char c = post[i];

    if (c == ' ')
      continue;
    if (c == 0)
      break;

    if (isdigit(c)) {
      num = read_double(post, i);
      st.push(num);
      --i;
      num = 0;
      continue;
    }

    double y = st.top();
    st.pop();

    if (c == '|') {
      st.push(cos(y));
      continue;
    }

    double x = st.top();
    st.pop();
    double z;

    if (c == '+')
      z = x + y;
    else if (c == '-')
      z = x - y;
    else if (c == '*')
      z = x * y;
    else if (c == '/')
      z = x / y;
    else if (c == '^') {
      z = pow(x, y);
    }

    st.push(z);
  }

  return st.top();
}

#endif  // algo.h
