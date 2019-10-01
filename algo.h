#ifndef _CALCALGO_H
#define _CALCSMATH_H
#define _CALCALGO_H

#include <QDebug>
#include <functional>
#include <string>
#include "hashmap.h"
#include "mainwindow.h"
#include "smath.h"
#include "stack.h"

const double EPS = 1e-8;
const double PI = 3.14159265358979323846;
const double EULER = 2.7182818284590452353602874713527;
const double INF = std::numeric_limits<double>::max();

const char OPERATIONS[5] = {'+', '-', '*', '/', '^'};
std::function<void(std::string)> error;

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
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

bool opening(std::string c) {
  return c == "(" || c == "[";
}

bool closing(std::string c) {
  return c == ")" || c == "]";
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
    d = d * 10.0 + int(c - '0');
    if (d >= INF) {
        error("Overflowing number given");
        return INF;
    }

    if (++pos < str.size())
      c = str[pos];
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
      error("Broken value given");
      return INF;
    }

    if (d * pow(10, man) > INF) {
        error("Overflowing number given");
        return NAN;
    }

    return d * pow(10, man);
  } else if (str[pos] != '.') {
    return d;
  }

  ++pos;
  if (pos >= str.size()) {
    pos = 0;
    error("Broken value given");
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

  int nnow = pos;
  if (pprev == nnow) {
    pos = 0;
    error("Broken value given");
    return INF;
  }

  if (pos < str.size() - 1 && (str[pos] == 'e' || str[pos] == 'E')) {
    ++pos;
    double v = read_single(str, pos);
    return d * pow(10, v);
  }

  if (pos < str.size() && str[pos] == '.') {
    error("Broken value given");
    pos = 0;
    return INF;
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

void put_brackets(std::string& s, std::string& ans) {
  bool* to_close = new bool[s.size() + 1];
  for (int i = 0; i < s.size() + 1; i++)
    to_close[i] = false;

  int end_brackets = 0;
  for (int i = 1; i < s.size(); i++) {
    char c = s[i];

    if (to_close[i])
      ans += std::string(1, ')');

    if (isspace(c))
      continue;

    int end = 0;
    if ((opening(std::string(1, s[i - 1])) || iskey(s[i - 1]) || s[i - 1] == ',') &&
        (c == '+' || c == '-')) {
      int old = i;
      double d = read_double(s, i);

      int diff = i - old;
      if (diff <= 0) {
        i = old;
        int balance = 1;
        for (int j = i + 1; j < s.size() - 1; j++) {
          if (opening(std::string(1, s[j]))) {
            balance++;
          } else if (closing(std::string(1, s[j]))) {
            balance--;
          } else
            continue;

          if (balance == 0) {
            if (j >= s.size())
              end_brackets++;
            else
              to_close[j] = true;
            break;
          }
        }

        if (balance > 0)
          end_brackets += balance;

        ans += "(0";
        ans += c;
        continue;
      }

      ans += "(0";
      ans += c;

      if (d < INF)
        ans += std::to_string(abs(d));
      ans += ")";
      --i;
    } else {
      ans += c;
    }
  }

  for (int i = 0; i < end_brackets; i++)
    ans += ")";
}

bool check_brackets(std::string& ans) {
  for (int i = 0; i < ans.size() - 1; i++) {
    if (opening(std::string(1, ans[i])) &&
        closing(std::string(1, ans[i + 1]))) {
      error("Empty brackets found");
      return false;
    }
  }
  return true;
}

void trim_unary(std::string& s,
                hashmap<std::string, double, string_hash>& vars,
                stack<std::string>& st) {
  if (s.empty()) {
      error("Empty string");
      return;
  }
  std::string first;
  for (int i = 0; i < s.size(); i++) {
    int before = i;
    std::string name = get_word(s, i);
    if (i == before) {
      first += s[i];
      continue;
    }

    if (!vars.contains(name)) {
      first += name;
      if (name != "sin" && name != "cos" && name != "log")
        st.push(name);
      --i;
      continue;
    }

    if (i != before) {
      if (isdigit(s[before - 1])) {
        error("Ommited multiplication sign before constant");
        return;
      }
      first += std::to_string(vars.get(name));
      --i;
    } else {
      first += s[i];
    }
  }

  s = "(" + first + ")";
  std::string ans;
  ans = "(";

  put_brackets(s, ans);
  if (!check_brackets(ans))
    return;

  s = ans;
  qDebug() << "We have pre-proccessed = " << QString::fromStdString(s) << endl;
}

void to_polish(const std::string& inp,
               std::string& outp,
               hashmap<std::string, double, string_hash>& vars) {
  int intro = 0;
  stack<std::string> st;

  do {
    char c = inp[intro];
    bool success = false;
    switch (c) {
      case '+':
      case '-':
        while (!st.empty()) {
          std::string op = st.top();
          if (!opening(op)) {
            outp = outp + op + " ";
            st.pop();
          } else
            break;
        }

        st.push(std::string(1, c));
        break;

      case '*':
      case '/':
        while (!st.empty()) {
          std::string op = st.top();
          if (op == "^" || op == "*" || op == "/" || op == "sin" ||
              op == "cos" || op == "log") {
            outp = outp + op + " ";
            st.pop();
          } else
            break;
        }
        st.push(std::string(1, c));
        break;

      case '(':
      case '[':
        st.push(std::string(1, c));
        break;

      case ')':
        success = false;
        while (!st.empty()) {
          std::string op = st.top();
          st.pop();
          if (op == "(") {
            success = true;
            break;
          } else
            outp += op + " ";
        }

        if (!success) {
          error("broken bracket balance, need pair bracket for )");
          return;
        }
        break;

      case ']':
        success = false;
        while (!st.empty()) {
          std::string op = st.top();
          st.pop();
          if (op == "[") {
            success = true;
            break;
          } else
            outp += op + " ";
        }
        if (!success) {
          error("broken bracket balance, need pair bracket for ]");
          return;
        }
        break;

      case ',':
        success = false;
        while (!st.empty()) {
          std::string op = st.top();
          st.pop();
          if (op == "(") {
            success = true;
            break;
          } else
            outp += op + " ";
        }

        if (!success) {
          error("missing ( for binary function");
          return;
        }

        st.push(std::string(1, '('));
        break;

      case '^':
        while (!st.empty()) {
          std::string op = st.top();
          if (op == "sin" || op == "cos" || op == "log") {
            outp = outp + op + " ";
            st.pop();
          } else
            break;
        }
        st.push(std::string(1, c));
        break;

      default:
        std::string str;
        if (isdigit(c)) {
          // number
          int was = intro;
          str = std::to_string(read_double(inp, intro));
          int stands = intro;
          if (stands - was <= 0) {
            error("Broken value given");
            return;
          }
        } else {
          // variable
          int was = intro;
          std::string name = get_word(inp, intro);
          int stands = intro;
          if (stands - was <= 0) {
            qDebug() << "lol " << inp[was] << ' ' << inp[was - 1] << endl;
            qDebug() << QString::fromStdString(inp) << endl;
            error("Wrong expression near \"" + std::string(1, inp[was]) + "\"" +
                  " (at " + std::to_string(was) + ") ");
            return;
          }

          if (vars.contains(name))
            str = std::to_string(vars.get(name));
          else {
            // function
            if (name != "sin" && name != "cos" && name != "log") {
              error("no such function/operator or variable " + name);
              return;
            }

            if (inp[intro] != '(') {
              error("Illegal use of function. Expected ( after " + name);
              return;
            }

            while (!st.empty()) {
              std::string op = st.top();
              if (op == "sin" || op == "cos" || op == "log") {
                outp = outp + op + " ";
                st.pop();
              } else
                break;
            }
            st.push(name);
            continue;
          }
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

  int i;
  for (i = 0; i < post.size(); i++) {
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

    if (opening(std::string(1, c)) || closing(std::string(1, c))) {
      error("bracket balance broken, need pair bracket for " +
            std::string(1, c));
      return NAN;
    }

    if (st.empty()) {
      error("not enough arguments given for operation");
      return NAN;
    }

    double y = st.top();
    st.pop();

    if (post[i] == 's' && post[i + 1] == 'i' && post[i + 2] == 'n') {
      i += 2;
      st.push(sin(y));
      continue;
    }

    if (post[i] == 'c' && post[i + 1] == 'o' && post[i + 2] == 's') {
      i += 2;
      st.push(cos(y));
      continue;
    }

    if (st.empty()) {
      error("not enough arguments given for binary operation");
      return NAN;
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
    else if (c == '/') {
      if (abs(y) <= EPS) {
        error("Division by zero");
        return NAN;
      }
      z = x / y;
    } else if (c == '^')
      z = pow(x, y);
    else if (c == 'l' && post[i + 1] == 'o' && post[i + 2] == 'g') {
      i += 2;
      z = log(y) / log(x);
    }

    st.push(z);
  }

  if (st.empty()) {
    error("broken operand balance");
    return NAN;
  }

  if (st.size() >= 2) {
    error("broken operand balance");
    return NAN;
  }
  return st.top();
}

#endif  // algo.h
