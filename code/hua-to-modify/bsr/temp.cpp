#include <vector>
#include <iostream>
#include <algorithm>
#include "basic_functions.cpp"

using namespace std;

int main(){
		vector<int> v = {3,1,2};
		sort(v.begin(), v.end());
		printVector(v);
		do{
				printVector(v);
		}while(next_permutation(v.begin(), v.end()));
		printVector(v);
		return 0;
}
