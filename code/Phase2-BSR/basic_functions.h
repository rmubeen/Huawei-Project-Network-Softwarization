/*
 * Some functions which perform basic functionality required by the program.
 */

#ifndef BASIC_FUNCTIONS_H
#define BASIC_FUNCTIONS_H

#include <vector>
#include <string>

using namespace std;

/*
 * it either prints or not a given object to the std output depending on the status. It can be used for debugging.
 */
template <class T3>
void printPy(T3 obj, bool status);

/*
 * it prints a given object to the std output.
 */
template <class T4>
void printPy(T4 obj){
  printPy(obj, true);
}

/*
 * it either prints or not a given 1-D vector to the std output depending on the status. It can be used for debugging.
 */
template <class T2>
void printVector(vector<T2> vec, bool status);

/*
 * it prints a given 1-D vector to the std output.
 */
template <class T5>
void printVector(vector<T5> vec){
  printVector(vec, true);
}

/*
 * it either prints or not a given 2-D vector to the std output depending on the status. It can be used for debugging.
 */
template <class T1>
void print2DVector(vector<vector<T1>> vec, bool status);

/*
 * it prints a given 2-D vector to the std output.
 */
template <class T6>
void print2DVector(vector<vector<T6>> vec){
  print2DVector(vec, true);
}

/*
 * it converts a given number-string to the number and returns it.
 */
int strToInt(string str);

/*
 * it returns a combination of all integers in bases parameter whose total sum is equal sum parameter where number of integers is equal to size parameter.
 */
vector<vector<int>> intPermutationsHelper(int sum, vector<int> bases, int size);

/*
 * it returns a combination of all integers in vector [min+base*0, min+base*1, min+base*2, ...] whose total sum is equal sum parameter.
 */
vector<vector<int>> intPermutations(int min, int base, int sum);

/*
 * like, python dictionary search. it returns index of the pair in vector where first element in the pair is equal to given key.
 */
int findPairInVector(vector<pair<int, int>> vec, int key);

/*
 * it returns index of the element in vector which is equal to val parameter. And, if it doesn't find val in vector, it returns the next element.
 * which is greater than val parameter.
 */
int findIndexOfNextInVector(vector<int> vec, int val);

/*
 * it removes repited vectors in the 2-D vector and return the 2-D vector where each vector appears once.
 */
vector<vector<int>> remove_repition_in_vector(vector<vector<int>> set);

#endif
