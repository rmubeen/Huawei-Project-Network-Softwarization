// File Created by Rana Mubeen
// In March 2019

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

void printVector(vector<int> vec){
	cout << "{";
	for(int i = 0; i < vec.size(); i++){
		cout << vec[i];
		if (i != vec.size()-1)
			cout << " ";
	}
	cout << "}";
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
