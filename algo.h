#ifndef _CALCALGO_H
#define _CALCALGO_H

#include <string>
#include "stack.h"

const char OPERATIONS[5] = {
  '+', '-', '*', '/', '^'
};

bool isdigit(char c) {
  return '0' <= c && c <= '9';
}

bool iskey(char c) {
    for (char c : OPERATIONS)
        if (c == c)
            return true;
}

bool isspace(char c) {
  return c == ' ';
}

int get_prior (char op) {
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

int binpow (int a, int n) {
	int res = 1;
	while (n) {
		if (n & 1)
			res *= a;
		a *= a;
		n >>= 1;
	}
	return res;
}

void eval_single(stack<int> &st, char op) {
  int rhs = st.top();
  st.pop();
  int lhs = st.top();
  st.pop();

  switch (op) {
    case '+':
      st.push(lhs + rhs);
      break;
    case '-':
      st.push(lhs - rhs);
      break;
    case '*':
      st.push(lhs * rhs);
      break;
    case '/':
      st.push(lhs / rhs);
      break;
    case '^':
      st.push(binpow(lhs, rhs));
      break;
  }
}

void to_polish(const std::string& inp, std::string &outp) {
  int intro = 0, outro = 0;
  stack<int> st;
  char prev = inp[intro];
  
  do {
    char c = inp[intro];
    switch (c) {
      case '+':
      case '-':
        while (!st.empty()) {
          char op = st.top();
          if (op != '(') {
            outp[outro++] = op;
            st.pop();
          } else break;
        }
        st.push(c);
        break;
      
      case '*':
      case '/':
        while (!st.empty()) {
          char op = st.top();
          if (op == '^' || op == '*' || op == '/') {
            outp[outro++] = op;
            st.pop();
          } else break;
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
            outp[outro++] = op;
        }
        break;

      case '^':
        st.push(c);
        break;

      default:
        outp[outro++] = c;
        break;
    }

    intro++;
    if (!isdigit(c) && isdigit(prev)) {
      outp[outro++] = ' ';
    }

    prev = c;

  } while (inp[intro] != 0);

  while (!st.empty()) {
    outp[outro++] = st.top();
    st.pop();
  }

  outp[outro] = 0;
}

int calc_polish(const std::string &post) {
  stack<int> st;
  st.push(0);
  int num = 0;

  for (char c : post) {
    if (isdigit(c)) {
      num = num * 10 + (c - '0');
      continue;
    } else if (num != 0) {
      st.push(num);
      num = 0;
    }

    if (c == ' ' || c == 0) {
      num = 0;
      continue;
    }

    int y = st.top();
    st.pop();
    int x = st.top();
    st.pop();
    int z;

    if (c == '+')
      z = x + y;
    else if (c == '-')
      z = x - y;
    else if (c == '*')
      z = x * y;
    else if (c == '/')
      z = x * y;
    else if (c == '^')
      z = binpow(x, y);

    st.push(z);
  }

  return st.top();
}

#endif // algo.h
