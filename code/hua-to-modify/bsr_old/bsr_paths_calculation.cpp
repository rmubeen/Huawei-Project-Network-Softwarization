// File Created by Rana Mubeen
// In March 2019

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "reading_utilities.cpp"
#include "basic_functions.cpp"

using namespace std;

struct vLink_S{
	int id;
	string src;
	string dst;
	int requirement;
	bool status;
};




int findPairVector(vector<pair<int, int>> vec, int key){
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i].first == key){
			cout << "found me\n";
			return i;
		}
	}
	return -1;
}

int findSlicesForPermutationProtectedPaths(vector<int> bwPermutation, protectedPathsOfPair_S vLinkPaths, int i){
	vector<pair<int, int>> bwDiv;
	int allocatedBW;
	int tempInt=0;

	for (int j = 0; j < vLinkPaths.protectedPathsSet[i].size(); j++) {
		allocatedBW = bwPermutation[j]/(vLinkPaths.protectedPathsSet[i][j].size()-1);
		tempInt = allocatedBW % 50;
		if (tempInt > 0) {
			allocatedBW = allocatedBW - tempInt + 50;
		}

		for (int k = 0; k < vLinkPaths.protectedPathsSet[i][j].size(); k++) {
			tempInt = findPairVector(bwDiv, vLinkPaths.protectedPathsSet[i][j][k]);
			if (tempInt > -1)
				bwDiv[tempInt].second = (bwDiv[tempInt].second) + allocatedBW;
			else
				bwDiv.push_back(make_pair(vLinkPaths.protectedPathsSet[i][j][k], allocatedBW));
		}
	}

	printVector(bwPermutation);
	cout << endl;
	for (int j = 0; j < bwDiv.size(); j++) {
		cout << bwDiv[j].first << " " << bwDiv[j].second << endl;
	}

	return 0;
}

void solveEachProtectedPathSet(vLink_S vLink, protectedPathsOfPair_S vLinkPaths){
	int nOfProtectedPaths;
	vector<vector<int>> reqPermutations = intPermutations(100, 100, vLink.requirement);
	int slices = 0;

	for (int i = 0; i < vLinkPaths.protectedPathsSet.size(); i++) {
		nOfProtectedPaths = vLinkPaths.protectedPathsSet[i].size();
		for(int j = 0; j < reqPermutations.size(); j++) {
			if(reqPermutations[j].size() == nOfProtectedPaths){
				slices = findSlicesForPermutationProtectedPaths(reqPermutations[j], vLinkPaths, i);
			}
		}
		cout <<"next path"<< endl;
	}

}

int main(int argc, char** argv) {
	// VARIABLE DECLARATIONS

	// GETTING INTO BUSINESS
	for (int i = 0; i < vnLinks.size(); i++) {
		vLinkPaths = VNPaths->getPathsOfVLink(vnLinks[i].id);
		solveEachProtectedPathSet(vnLinks[i], vLinkPaths);
		break;
	}

	return 0;
}
