// MIT License
// Copyright (c) 2019 Marco Pascucci

#include <algorithm>
#include <assert.h>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

#include "costs.h"
#include "linalg.h"

template <typename Tx, typename Ty> struct OPResult {
  vector<unsigned int> cp;
  vector<Tx> x;
  vector<Ty> y;
  double cost;
  OPResult(){};
  OPResult<Tx, Ty>(vector<unsigned int> &cp, vector<Tx> &x, vector<Ty> &y,
                   double cost)
      : cp(cp), x(x), y(y), cost(cost){};
  // void operator=(OPResult<Tx,Ty> pr) {
  //     cp = pr.cp;
  //     x = pr.x;
  //     y = pr.y;
  //     cost = pr.cost;
  // }
};

// template <typename T1, typename T2>
// double sum_of_squares(vector<T1> &y, vector<T2> &y0, size_t start, size_t
// end) {
//   /* @brief Calculate sum of square errors between y and y0 */
//   if (y.size() != y0.size()) {
//     throw std::invalid_argument("a and b must have the same dimension");
//   }
//   if (start < 0 || end > y.size()) {
//     throw std::invalid_argument("invalid start/end range");
//   }
//   double sum_of_squares = 0;
//   for (size_t i = start; i < end; i++) {
//     sum_of_squares = sum_of_squares + pow((double)(y[i] - y0[i]), 2);
//   }
//   return sum_of_squares;
// }
// template <typename T1, typename T2>
// double sum_of_squares(vector<T1> &y, vector<T2> &y0) {
//   return sum_of_squares(y, y0, 0, y.size());
// }

vector<unsigned int> backtrack(vector<unsigned int> cp_raw) {
  /* @brief  backtrack the changepoint vector */
  unsigned int x = cp_raw[cp_raw.size() - 1];
  vector<unsigned int> cp;
  while (x > 0) {
    cp.push_back(x);
    x = cp_raw[x];
  }
  reverse(cp.begin(), cp.end());
  return cp;
}

template <typename Tx, typename Ty>
OPResult<Tx, Ty> op1D(vector<Tx> &x, vector<Ty> &y, double beta) {
  /* detect changepoints with OP algorithm */
  assert(x.size() == y.size());
  unsigned int n = y.size();
  unsigned int tau;
  vector<double> q(n);
  vector<unsigned int> P(1, 0);
  vector<unsigned int> temp;
  vector<unsigned int> cp_raw(n, 0);

  double this_cost, q_temp;
  double cp_temp = 0;
  vector<double> q_new;
  // cp_raw[0] = 0;

  for (size_t t = 1; t < n; ++t) { // t is in [0,n)
    // possible changepoint @ t

    q_temp = cost_constant(y, 0, t); // the cost of no cp in [0 t)
    // cp_temp = 0;
    q_new = vector<double>(t, 0);
    q_new[0] = q_temp;

    for (unsigned int i = 0; i < P.size(); i++) {
      tau = P[i];
      // for (unsigned int tau = 0; tau < t; tau++) { // tau is in [0,t)
      this_cost = cost_constant(y, tau, t); // the cost of no cp in [tau t)
      q_new[tau] = q[tau] + this_cost;      // q_new
      if (q_new[tau] + beta < q_temp) { // if it is convenient to break at tau
        q_temp = q_new[tau] + beta;
        cp_temp = tau;
      }
    }
    q[t] = q_temp;
    cp_raw[t] = cp_temp;

    //* pruning
    temp.clear();
    for (auto this_P : P) {
      if (q_new[this_P] < q_temp)
        temp.push_back(this_P);
    }
    temp.push_back(t);
    P = temp;

    P.push_back(t);
  }

  // construct result structure
  vector<unsigned int> cp = backtrack(cp_raw);
  vector<unsigned int> cp1(cp);
  cp1.insert(cp1.begin(), 0);
  cp1.push_back(n - 1);
  vector<Ty> ys((cp1.size() - 1) * 2);
  vector<Tx> xs((cp1.size() - 1) * 2);

  Tx xa, xb;
  Ty ya, yb;

  for (unsigned int i = 0; i < cp1.size() - 1; i++) {
    if (i == 0) {
      xa = x[cp1[i]];
    } else {
      xa = x[cp1[i]] + 1;
    }

    xb = x[cp1[i + 1]];

    xs[i * 2] = xa;
    xs[i * 2 + 1] = xb;
    ys[i * 2] = ya;
    ys[i * 2 + 1] = yb;
  }

  return OPResult<Tx, Ty>(cp, xs, ys, q[y.size() - 2]);
}

template <typename Tx, typename Ty>
OPResult<Tx, Ty> op2D(vector<Tx> &x, vector<Ty> &y, double beta) {
  /* detect changepoints with OP algorithm */
  assert(x.size() == y.size());
  unsigned int n = y.size();
  unsigned int tau;
  vector<double> q(n);
  vector<unsigned int> P(1, 0);
  vector<unsigned int> temp;
  vector<double> coeffs(n); // for result output
  vector<double> inters(n); // for result output
  vector<unsigned int> cp_raw(n, 0);
  double coeff_temp, inter_temp; // for result output

  double coeff, inter, this_cost, q_temp;
  double cp_temp = 0;
  vector<double> q_new;
  // cp_raw[0] = 0;

  for (size_t t = 1; t < n; ++t) {
    // possible changepoint @ t

    lin_reg(x, y, &coeff, &inter, 0, t);
    q_temp = cost_linear(x, y, coeff, inter, 0, t);
    // cp_temp = 0;
    q_new = vector<double>(t, 0);
    q_new[0] = q_temp;
    coeff_temp = coeff; // for result output
    inter_temp = inter; // for result output

    // return vector<int>(1,1);

    for (unsigned int i = 0; i < P.size(); i++) {
      tau = P[i];
      // for (unsigned int tau = 0; tau < t; tau++) { // tau is in [0,t)
      if (t - tau == 1)
        continue; // linear fit needs minimum 2 points
      lin_reg(x, y, &coeff, &inter, tau, t);
      this_cost = cost_linear(x, y, coeff, inter, tau, t);
      q_new[tau] = q[tau] + this_cost; // q_new
      if (q_new[tau] + beta < q_temp) {
        q_temp = q_new[tau] + beta;
        cp_temp = tau;
        coeff_temp = coeff; // for result output
        inter_temp = inter; // for result output
      }
    }
    q[t] = q_temp;
    cp_raw[t] = cp_temp;
    coeffs[t] = coeff_temp; // for result output
    inters[t] = inter_temp; // for result output

    temp.clear();

    for (size_t i = 0; i < P.size(); i++) {
      if (q_new[P[i]] < q_temp) {
        // printf("erased: %d", i);
        temp.push_back(P[i]);
      }
    }
    temp.push_back(t);
    P = temp;
    // printv(P);
  }

  // construct result structure
  vector<unsigned int> cp = backtrack(cp_raw);
  vector<unsigned int> cp1(cp);
  cp1.insert(cp1.begin(), 0);
  cp1.push_back(n - 1);
  vector<Ty> ys((cp1.size() - 1) * 2);
  vector<Tx> xs((cp1.size() - 1) * 2);

  Tx xa, xb;
  Ty ya, yb;

  for (unsigned int i = 0; i < cp1.size() - 1; i++) {
    xa = x[cp1[i]];
    if (i < cp1.size() - 2) {
      xb = x[cp1[i + 1] - 1];
    } else {
      xb = x[n - 1];
    }

    ya = coeffs[cp1[i + 1]] * xa + inters[cp1[i + 1]];
    yb = coeffs[cp1[i + 1]] * xb + inters[cp1[i + 1]];

    xs[i * 2] = xa;
    xs[i * 2 + 1] = xb;
    ys[i * 2] = ya;
    ys[i * 2 + 1] = yb;
  }

  return OPResult<Tx, Ty>(cp, xs, ys, q[y.size() - 2]);
}

template <typename Tx, typename Ty>
OPResult<Tx, Ty> op2Dcc(vector<Tx> &x, vector<Ty> &y, double beta) {
  /* detect changepoints with OP algorithm */
  assert(x.size() == y.size());
  unsigned int n = y.size();
  unsigned int tau;
  vector<double> q(n);
  vector<unsigned int> cp_raw(n, 0);
  vector<unsigned int> P(1, 0);
  vector<unsigned int> temp;
  vector<Tx> x_trans(y.size());
  vector<Ty> y_trans(y.size());
  vector<Ty> y_end(y.size());
  vector<Ty> y_start(y.size());

  double coeff, coeff_temp, inter, this_cost, q_temp, cp_temp;
  Point<Tx, Ty> pt_temp(0, 0), pt(0, 0);
  vector<double> q_new;

  cp_raw[0] = 0;

  for (size_t t = 1; t < n; ++t) {
    // possible changepoint @ t

    lin_reg(x, y, &coeff, &inter, 0, t);
    q_temp = cost_linear(x, y, coeff, inter, 0, t);
    cp_temp = 0;
    q_new = vector<double>(t, 0);
    q_new[0] = q_temp;
    coeff_temp = coeff;
    pt.x = x[0];
    pt.y = coeff * x[0] + inter;
    pt_temp = pt;

    // return vector<int>(1,1);

    for (unsigned int i = 0; i < P.size(); i++) {
      tau = P[i];
      if (t-tau == 1) continue;
      pt.x = x[tau];
      pt.y = y_end[tau];
      transform(x.begin(), x.end(), x_trans.begin(),
                bind2nd(std::plus<int>(), -pt.x));
      transform(y.begin(), y.end(), y_trans.begin(),
                bind2nd(std::plus<double>(), -pt.y));
      lin_reg_no_const(x_trans, y_trans, &coeff, tau, t);
      this_cost = cost_linear_point(x, y, pt, coeff, tau, t);
      q_new[tau] = q[tau] + this_cost; // q_new
      if (q_new[tau] + beta < q_temp) {
        q_temp = q_new[tau] + beta;
        cp_temp = tau;
        coeff_temp = coeff;
        pt_temp = pt;
      }
    }
    q[t] = q_temp;
    cp_raw[t] = cp_temp;
    y_start[t] = coeff_temp * (x[cp_temp] - pt_temp.x) + pt_temp.y;
    y_end[t] = coeff_temp * (x[t] - pt_temp.x) + pt_temp.y;
    temp.clear();

    for (size_t i = 0; i < P.size(); i++) {
      if (q_new[P[i]] < q_temp) {
        // printf("erased: %d", i);
        temp.push_back(P[i]);
      }
    }
    temp.push_back(t);
    P = temp;
    // printv(P);
  }

  // construct result structure
  vector<unsigned int> cp = backtrack(cp_raw);
  vector<unsigned int> cp1(cp);
  cp1.insert(cp1.begin(), 0);
  cp1.push_back(n - 1);
  vector<Ty> ys(cp1.size());
  vector<Tx> xs(cp1.size());

  for (unsigned int i = 0; i < cp1.size(); i++) {
    xs[i] = x[cp1[i]];
    ys[i] = y_end[cp1[i]];
  }
  ys[0] = y_start[cp1[1]];

  return OPResult<Tx, Ty>(cp, xs, ys, q[n - 1]);
}
