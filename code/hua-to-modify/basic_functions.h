#ifndef BASIC_FUNCTIONS_H
#define BASIC_FUNCTIONS_H

#include <vector>
#include <string>

using namespace std;

template <class T3>
void printPy(T3 obj, bool status);
template <class T4>
void printPy(T4 obj){
  printPy(obj, true);
}

template <class T2>
void printVector(vector<T2> vec, bool status);
template <class T5>
void printVector(vector<T5> vec){
  printVector(vec, true);
}

template <class T1>
void print2DVector(vector<vector<T1>> vec, bool status);
template <class T6>
void print2DVector(vector<vector<T6>> vec){
  print2DVector(vec, true);
}

int strToInt(string str);

vector<vector<int>> intPermutationsHelper(int sum, vector<int> bases, int size);
vector<vector<int>> intPermutations(int min, int base, int sum);

#endif
