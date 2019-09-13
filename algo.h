#ifndef _CALCALGO_H
#define _CALCALGO_H

#include <string>
#include "stack.h"
#include "smath.h"

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

double read_single (const std::string &str, int &pos) {
  double d = 0;
  char c = str[pos];

  bool sign = false;
  if (c == '-' || c == '+') {
    ++pos;
    sign = c == '-';
    c = str[pos];
  }

  while (pos < str.size() && isdigit(c)) {
    d = d * 10.0 + (c - '0');
    c = str[++pos];
  }
  return (sign ? -1.0 : 1.0) * d;
}

double read_double (const std::string &str, int &pos) {
  double d = read_single(str, pos);
  if (pos >= str.size())
    return d;

  if (str[pos] == 'e') {
    ++pos;
    double man = read_single(str, pos);
    return d * pow(10, man);
  } else if (str[pos] != '.') {
    return d;
  }

  ++pos;
  if (pos >= str.size()) {
    return d;
  }

  char c = str[pos];
  double ten = 10.0;
  while (pos < str.size() && isdigit(c)) {
    d += double(c - '0') / ten;
    ten *= 10;
    c = str[++pos];
  }

  return d;
}

void to_polish(const std::string& inp, std::string& outp) {
  int intro = 0, outro = 0;
  stack<char> st;

  do {
    char c = inp[intro];
    switch (c) {
      case '+':
      case '-':
        while (!st.empty()) {
          char op = st.top();
          if (op != '(') {
            outp = outp + op, outro++;
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
            outp = outp + op, outro++;
            st.pop();
          } else
            break;
        }
        st.push(c);
        break;

      case '(':
        st.push(c);
        break;

      case ')':
        while (!st.empty()) {
          char op = st.top();
          st.pop();
          if (op == '(')
            break;
          else
            outp = outp + op, outro++;
        }
        break;

      case '^':
        st.push(c);
        break;

      default:
        if (isdigit(c)) {
          std::string str = std::to_string(read_double(inp, intro));
          --intro;
          outp = outp + str + ' ';
          outro += (str + ' ').size();
        } else {
          outp = outp + c;
          outro++;
        }
        break;
    }

    intro++;

  } while (intro < inp.size());

  while (!st.empty()) {
    outp = outp + st.top();
    outro++;
    st.pop();
  }

  outp[outro] = 0;
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
