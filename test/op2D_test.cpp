#include "test_tools.hpp"
#include <iostream>
#include <op.h>
#include <vector>

using namespace std;

int main() {

  //* TEST LINEAR REGRESSION
  vector<int> x(5);
  range(x);
  vector<double> y1(x.size());
  vector<double> y2(x.size());
  double m0, q0, m, q;
  m0 = 2;
  q0 = 3;
  y1 = line(x, m0);
  y2 = line(x, m0, q0);
  lin_reg_no_const(x, y1, &m);
  assert(m == m0);
  lin_reg(x, y2, &m, &q);
  assert(m == m0 && q == q0);

  //* TEST sum_of_squares
  // y1 = {1, 2, 3};
  // y2 = {2, 3, 4};
  // assert(sum_of_squares(y1, y2) == 3);

  //* TEST CONSTANT CONST
  y1 = vector<double> ({1,1,1,1,1,1});
  assert(0 == cost_constant<double>(y1));
  y1 = vector<double> ({0,1,0,-1});
  assert(2 == cost_constant<double>(y1));
  y1 = vector<double> ({0,1,0,0});
  assert(0.75 == cost_constant<double>(y1));

  //* TEST COST LINEAR
  x = range<int>(10);
  m0 = 2;
  q0 = 3;
  y1 = line(x, m0, q0);
  lin_reg(x, y1, &m, &q);
  // printf("m: %f q: %f\n", m, q);
  assert(0 == cost_linear(x, y1, m, q));

  vector<int> err(y1.size(), 1);
  transform(y1.begin(), y1.end(), err.begin(), y1.begin(), std::plus<double>());
  assert(cost_linear(x, y1, m, q) == 10);

  // // TEST op2D
  // vector<double> y3 = vector<double>(
  //     {26.83673874, 29.70238512, 47.2053602,  50.45076346, 53.16091091,
  //      58.63083843, 74.16741128, 75.93466904, 86.9642007,  78.13685392,
  //      76.89526256, 68.4776131,  65.1940033,  65.11475645, 66.4528478,
  //      56.46789428, 60.43826339, 57.16601003, 56.57857668, 43.50349989});

  // vector<double> x3 = range<double>(y3.size());
  // // printf("size: %d\n", x3.size());
  // // printVector(y3);

  // // printVector(op2D(x3,y3,500));
  // cout << ("op2D TEST : attended [ 8 ]:") << endl;
  // printVector(backtrack(op2D(x3, y3, 500).cp)); // attended
  // cout << endl;

  // // TEST cost_linear_point

  // vector<double> y4 = {4, 5, 6, 7};

  // vector<double> y4_temp(y4);
  // vector<int> x4 = range<int>(y4.size());
  // vector<int> x4_temp(x4);

  // double m4;
  // Point<int, double> p(x4[0], y4[0]);
  // transform(x4.begin(), x4.end(), x4_temp.begin(),
  //           bind2nd(std::plus<int>(), -p.x));
  // transform(y4.begin(), y4.end(), y4_temp.begin(),
  //           bind2nd(std::plus<double>(), -p.y));
  // lin_reg_no_const(x4_temp, y4_temp, &m4);
  // // printf("m: %f\n", m4);
  // assert(cost_linear_point(x4, y4, p, m4) == 0); // attended [ 8 ]

  // // TEST op2Dcc
  // vector<double> y5 = vector<double>(
  //     {66.98529725, 70.57768284, 71.19654363, 75.04147585, 70.03110319,
  //      77.63135952, 76.56212064, 80.92867312, 80.51872377, 84.63607764,
  //      84.33052602, 83.69384519, 75.72599947, 62.29764306, 52.78959841,
  //      44.40018376, 34.99630724, 25.9742604,  32.64711626, 40.16358948});
  // vector<double> y5_temp(y5);
  // vector<int> x5 = range<int>(y5.size());
  // vector<int> x5_temp(x5);

  // double m5;
  // Point<int, double> p5(x5[0], y5[0]);
  // transform(x5.begin(), x5.end(), x5_temp.begin(),
  //           bind2nd(std::plus<int>(), -p5.x));
  // transform(y5.begin(), y5.end(), y5_temp.begin(),
  //           bind2nd(std::plus<double>(), -p5.y));
  // lin_reg_no_const(x5_temp, y5_temp, &m5);
  // // printf("m: %f\n", m5);
  // // printVector(op2Dcc(x5,y5,500));
  // cout << ("op2D TEST : attended [ 11, 17 ]:") << endl;
  // printVector(backtrack(op2Dcc(x5, y5, 500).cp)); // attended [ 17, 11 ]
  // cout << endl;

  return 0;
}