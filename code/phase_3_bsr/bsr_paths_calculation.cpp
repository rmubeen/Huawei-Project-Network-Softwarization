// File Created by Rana Mubeen
// In March 2019

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "hua_disjoint_paths.cpp"
#include "reading_utilities.cpp"
using namespace std;

struct vLink_S{
	int id;
	string src;
	string dst;
	int requirement;
	bool status;
};

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

void setVLinkIDsInProtectedPathsOfVN(vector<vLink_S> vnLinks, protectedPathsOfVN_C* VNPaths){
	for (int i = 0; i < vnLinks.size(); i++) {
		VNPaths->setVLinkID(vnLinks[i].src, vnLinks[i].dst, vnLinks[i].id);
	}
}

vector<vLink_S> readVNLinks(reading_utilities readLine, string filename){
	vector<vLink_S> vnLinks;
	vLink_S temp;
	temp.status = false;
	string line;
	ifstream infile;
	infile.open(filename);
	if(!infile)
		cout << "ERR: error opening file " << filename << endl;

	while(getline(infile, line)) {
		temp.src = readLine.get_nth_item(line, 2, ",");
		temp.dst = readLine.get_nth_item(line, 1, ",");
		temp.id = strToInt(readLine.get_nth_item(line, 0, ","));
		temp.requirement = strToInt(readLine.get_nth_item(line, 5, ","));
		vnLinks.push_back(temp);
	}

	return vnLinks;
}

void solveEachProtectedPathSet(vLink_S vLink, protectedPathsOfPair_S vLinkPaths){

}

int main(int argc, char** argv) {
	// VARIABLE DECLARATIONS
	reading_utilities readLine;
	string rootFile = argv[1];
	protectedPathsOfPair_S vLinkPaths;

	// PREPEARING INITIAL DATA
	vector<vLink_S> vnLinks = readVNLinks(readLine, rootFile);
	string tempStr = rootFile + ".path";
	protectedPathsOfVN_C* VNPaths = new protectedPathsOfVN_C(tempStr, 4, 5);
	setVLinkIDsInProtectedPathsOfVN(vnLinks, VNPaths);
	VNPaths->printProtectedPathsOfVN(argv[2]);
	VNPaths->printStats();
	cout << endl;

	// GETTING INTO BUSINESS
	for (int i = 0; i < vnLinks.size(); i++) {
		vLinkPaths = VNPaths->getPathsOfVLink(vnLinks[i].id);
		solveEachProtectedPathSet(vnLinks[i], vLinkPaths);
	}

	return 0;
}
