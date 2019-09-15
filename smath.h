#ifndef _CALCSMATH_H
#define _CALCSMATH_H
#include <iostream>

const double EPS = 1e-8;
const double PI = 3.14159265358979323846;
const double EULER = 2.7182818284590452353602874713527;
const double INF = 1e20;

double fabs(double x) {
  return *(((int*)&x) + 1) &= 0x7fffffff;
}

double exp (double t) {
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
  } while (fabs(diff) >= EPS);
  return value;
}

double cos(const double x) {
  double val = 1.0, temp = 1.0;
  unsigned step = 1;
  for (; fabs(temp) >= EPS; step++) {
    temp *= -x * x / ((2.0 * step) * (2.0 * step - 1.0));
    val += temp;
  }
  return val;
}

double sin(const double x) {
  double val = x, temp = x;
  unsigned step = 1;
  for (; fabs(temp) >= EPS; step++) {
    temp *= -x * x / ((2.0 * step) * (2.0 * step + 1.0));
    val += temp;
  }
  return val;
}

double log(const double x) {
  double kpow = (x - 1.0) / (x + 1.0),
         kpow2 = kpow * kpow, dk = 0, n = 1,
         k = 2 * kpow;
  do {
    n += 2;
    kpow *= kpow2;
    dk = 2 * kpow / n;
    k += dk;
  } while ( fabs(dk) >= EPS );

  return k;
}

double log(double a, double x) {
  return log(x) / log(a);
}

double pow (double a, double p) {
  std::cout << "pow not stable " << a << "^" << p << std::endl;
  if (fabs(a) <= EPS)
    return 0.0;
  if (fabs(p) <= EPS)
    return 1.0;
  if (fabs(p) - 1.0 <= EPS)
    return a;
  if (p < 0)
    return 1 / pow(a, -p);
  return exp(log(a) * p);
}

#endif // smath.h
