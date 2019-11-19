// MIT License
// Copyright (c) 2019 Marco Pascucci

#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

template <typename Tx, typename Ty> struct Point {
  Tx x;
  Ty y;
  Point();
  Point(Tx x, Ty y) : x(x), y(y){};
  Point(Point<Tx, Ty> &p) : x(p.x), y(p.y){};
  void operator=(Point<Tx, Ty> &p) {
    x = p.x;
    y = p.y;
  }
};

template <typename T>
double cost_constant(vector<T> &y, size_t start, size_t end,
                     bool average = false) {
  assert(start <= end);
  /* Calculate the sum of the square error of y against its mean.
  From start (included) to end (excluded)
  */
  float ave =
      accumulate(y.begin() + start, y.begin() + end, 0.0) / (end - start);

  double cost = 0;
  for (unsigned int i = start; i < end; i++) {
    cost = cost + pow(y[i] - ave, 2);
  }
  if (average)
    cost = cost / (end - start);
  return cost;
}
template <typename T> double cost_constant(vector<T> &y, bool average = false) {
  return cost_constant(y, 0, y.size(), average);
}

template <typename T1, typename T2>
double cost_linear(vector<T1> &x, vector<T2> &y, double coeff, double inter,
                   size_t start, size_t end, bool average = false) {
  /* calculate the sum of square errors between y and x*coeff+inter 
  from start (included) to end (excluded) */
  assert(y.size() == x.size());
  assert(start >= 0 && end <= x.size());

  double cost = 0;
  for (unsigned int i = start; i < end; i++) {
    cost = cost + pow((y[i] - (x[i] * coeff + inter)), 2);
  }
  if (average)
    cost = cost / x.size();
  return cost;
}
template <typename T1, typename T2>
double cost_linear(vector<T1> &x, vector<T2> &y, double coeff, double inter,
                   bool average = false) {
  return cost_linear(x, y, coeff, inter, 0, x.size(), average);
}

template <typename T1, typename T2>
double cost_linear_point(vector<T1> &x, vector<T2> &y, Point<T1, T2> p,
                         double coeff, size_t start, size_t end,
                         bool average = false) {
  /* calculate the sum of square errors between y and x*coeff+inter */
  assert(y.size() == x.size());
  assert(start >= 0 && end <= x.size());

  double cost = 0;
  for (unsigned int i = start; i < end; i++) {
    cost = cost + pow((y[i] - ((x[i] - p.x) * coeff + p.y)), 2);
  }
  if (average)
    cost = cost / x.size();
  return cost;
}
template <typename T1, typename T2>
double cost_linear_point(vector<T1> &x, vector<T2> &y, Point<T1, T2> p,
                         double coeff, bool average = false) {
  return cost_linear_point(x, y, p, coeff, 0, x.size(), average);
}