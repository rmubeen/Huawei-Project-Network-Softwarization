// File Created by Rana Mubeen
// In March 2019
#ifndef MY_BASIC_FUNCTIONS_CPP
#define MY_BASIC_FUNCTIONS_CPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "basic_functions.h"
using namespace std;

template <class T3>
void printPy(T3 obj, bool status){
	if(!status)
		return;

	cout << obj;
}

template <class T2>
void printVector(vector<T2> vec, bool status){
	if(!status)
		return;

	cout << "{";
	for(int i = 0; i < vec.size(); i++){
		cout << vec[i];
		if (i != vec.size()-1)
			cout << " ";
	}
	cout << "}\n";
}

template <class T1>
void print2DVector(vector<vector<T1>> vec, bool status){
	if(!status)
		return;

	for (int i = 0; i < vec.size(); i++) {
		printVector(vec[i]);
	}
}

int strToInt(string str){
	int number = 0;

	for(int i=0; i < str.length(); i++){
		if(str[i] == '.')
			break;
		else
			number = (number * 10) + (int(str[i]) - 48);
	}

	return number;
}

vector<vector<int>> intPermutationsHelper(int sum, vector<int> bases, int size){
	if(bases.size() == 0){
		for (int i = 1; i <= sum; i++) {
			bases.push_back(i);
		}
	}

	vector<vector<int>> results = {};
	vector<vector<int>> newResults;

	if(size == 0){
		results = {};
	} else if(size==1) {
		if(find(bases.begin(), bases.end(), sum) != bases.end())
			results = {{sum}};
		else
			results = {};
	} else {
		for (int i = 0; i < bases.size(); i++) {

			if(bases[i] <= sum){
				newResults = intPermutationsHelper(sum-bases[i], bases, size-1);
				for (int j = 0; j < newResults.size(); j++) {
					newResults[j].push_back(bases[i]);
					results.push_back(newResults[j]);
				}
			}

		}
	}

	return results;
}

vector<vector<int>> intPermutations(int min, int base, int sum){
	vector<int> bases;
	vector<vector<int>> newResults;
	vector<vector<int>> results;

	for (int i = min; i <= sum; i += base)
		bases.push_back(i);

	for (int i = 1; i <= sum/min; i++) {
		newResults = intPermutationsHelper(sum, bases, i);
		for (int j = 0; j < newResults.size(); j++)
			results.push_back(newResults[j]);
	}

	return results;
}

int findIndexOfNextInVector(vector<int> vec, int val){

	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] >= val) {
			return i;
		}
	}

	return -1;
}

int findPairInVector(vector<pair<int, int>> vec, int key){
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i].first == key){
//			cout << "found me\n";
			return i;
		}
	}
	return -1;
}

bool is_in_vector(int a, vector<int> b){
	for(int i = 0; i < b.size(); i++){
		if (a == b[i])
			return true;
	}

	return false;
}

bool are_vec_equal(vector<int> one, vector<int> two){
	if(one.size() != two.size())
		return false;

	bool status = true;
	int size = one.size();

	for(int i = 0; i < size; i++){
		status = status && is_in_vector(one[i], two);
	}

	return status;
}

vector<vector<int>> remove_repition_in_vector(vector<vector<int>> set){
	int size = set.size();
	int i = 0;

	while(i < size){
			for(int j = i+1; j < size; j++){
				if (are_vec_equal(set[i], set[j])){
					set.erase(set.begin()+i);
					i--;
					size--;
					break;
				}
			}
			i++;
	}
	return set;
}
#endif
