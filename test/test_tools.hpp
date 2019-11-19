#include <iostream>
#include <vector>

using namespace std;

template <typename T>
void printVector(vector<T> v) {
  cout << "[ ";
  for (size_t i = 0; i < v.size(); i++) {
    cout << v[i];
    if (i != v.size() - 1)
      cout << ", ";
  }
  cout << " ]" << endl;
}