// File Created by Rana Mubeen
// In March 2019

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "hua_disjoint_paths.cpp"
using namespace std;

int main(int argc, char** argv) {

	protectedPathsOfVN_C temp(argv[1], 4, 5);
//	cout << "CP1\n";
	temp.printProtectedPathsOfVN(argv[2]);
//	cout << "CP2\n";
	temp.printStats();

	return 0;
}
