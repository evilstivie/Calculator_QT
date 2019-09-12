#ifndef _CALCSMATH_H
#define _CALCSMATH_H

const double EPS = 1e-8;
const double PI = 3.14159265358979323846;

double fabs(double x) {
  return *(((int*)&x) + 1) &= 0x7fffffff;
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

#endif // smath.h
