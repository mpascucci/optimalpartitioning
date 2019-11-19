#include <iostream>
#include <op.h>
#include <vector>
#include "test_tools.hpp"

using namespace std;

int main() {
   OPResult<int, double> r;

   // GENERATE SIGNAL
   vector<double> y = vector<double>({
      52.84906138, 42.31577059, 31.7824798 , 21.24918901, 10.71589822,
      4.38262669, 10.64941294, 16.91619919, 23.18298544, 29.44977169,
      35.71655794, 41.98334419, 48.25013043, 54.51691668, 60.78370293,
      67.05048918, 73.31727543, 69.76445339, 64.4787593 , 59.1930652 });

   vector<int> x(y.size());
   iota (x.begin(), x.end(), 0); // fill with 0,1,2...N


   //* TEST op2D
   cout << ("op2D TEST : attended [ 5, 17 ]:") << endl;
   r = op2D(x,y,10);
   printVector(r.cp);
   cout << endl;

   //* TEST op2Dcc (with continuity constraint)
   cout << ("op2Dcc TEST : attended [ 5, 17 ]:") << endl;
   r = op2Dcc(x,y,100);
   printVector(r.cp);
   cout << endl;

   //* TEST op1D
   cout << ("TEST op1D : attended [ 6, 10 ]:") << endl;
   y = vector<double>({1,1,1,1,1,1,50,50,50,50,30,30,30});
   x = vector<int>(y.size());
   iota (x.begin(), x.end(), 0); // fill with 0,1,2...N
   r = op1D(x,y,100);
   printVector(r.cp);
   cout << endl;
   
   return 0;
}