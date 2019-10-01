#ifndef _CALCSMATH_H
#define _CALCSMATH_H
#include <iostream>
#include <limits>
#include "algo.h"

// const static auto NAN = std::numeric_limits<double>::quiet_NaN();

double _abs(const double x) {
  uint64_t i = reinterpret_cast<const std::uint64_t&>(x);
  i &= 0x7FFFFFFFFFFFFFFFULL;
  return reinterpret_cast<const double&>(i);
}

double exp(double t) {
  double value = 1.0, before = value, term = 1.0, diff = 0, c = 0;
  unsigned it = 0, n = 1;
  do {
    it++;
    term *= t / n;
    double y = term - c;
    double p = value + y;
    c = (p - value) - y;
    value = p;
    n++;
    double after = value;
    diff = (before - after) / before;
    before = after;
  } while (_abs(diff) >= EPS);
  return value;
}

double cos(const double x) {
  double val = 1.0, temp = 1.0;
  unsigned step = 1;
  for (; _abs(temp) >= EPS; step++) {
    temp *= -x * x / ((2.0 * step) * (2.0 * step - 1.0));
    val += temp;
  }
  return val;
}

double sin(const double x) {
  double val = x, temp = x;
  unsigned step = 1;
  for (; _abs(temp) >= EPS; step++) {
    temp *= -x * x / ((2.0 * step) * (2.0 * step + 1.0));
    val += temp;
  }
  return val;
}

double log(const double x) {
  if (x == 1)
    return 0;
  if (x == 0)
    return INF;
  if (x < 0)
    return NAN;
  double kpow = (x - 1.0) / (x + 1.0), kpow2 = kpow * kpow, dk = 0, n = 1,
         k = 2 * kpow;
  do {
    n += 2;
    kpow *= kpow2;
    dk = 2 * kpow / n;
    k += dk;
  } while (_abs(dk) >= EPS);

  return k;
}

double log(double a, double x) {
  return log(x) / log(a);
}

double pow(double x, double y) {
  double ans;
  if (_abs(y) <= EPS)
    ans = 1;
  else if (_abs(y) - 1.0 <= EPS)
    ans = x;
  else if (y == NAN)
    ans = NAN;
  else if (_abs(x) > 1 && y >= INF)
    ans = INF;
  else if (_abs(x) > 1 && y <= -INF)
    ans = 0;
  else if (_abs(x) < 1 && y >= INF)
    ans = 0;
  else if (_abs(x) < 1 && y <= -INF)
    ans = INF;
  else if (_abs(x) - 1.0 <= EPS && _abs(y) >= INF)
    ans = NAN;
  else if (_abs(x) <= EPS && y != 0 && y != NAN)
    ans = 0;
  else if (-_abs(x) <= EPS && y != 0 && y != NAN && int(y) % 2 == 0)
    ans = INF;
  else if (y < 0)
    ans = 1 / pow(x, -y);
  else
    ans = exp(log(x) * y);
  return ans;
}

#endif  // smath.h
