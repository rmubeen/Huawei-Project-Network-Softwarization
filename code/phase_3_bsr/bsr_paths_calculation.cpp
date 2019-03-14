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

void setVLinkIDsInProtectedPathsOfVN(reading_utilities readLine, string filename, protectedPathsOfVN_C* VNPaths){
	string src;
	string dst;
	string id;
	string line;
	ifstream infile;
	infile.open(filename);
	if(!infile)
		cout << "ERR: error opening file " << filename << endl;

	while(getline(infile, line)) {
		src = readLine.get_nth_item(line, 2, ",");
		dst = readLine.get_nth_item(line, 1, ",");
		id = readLine.get_nth_item(line, 0, ",");
		VNPaths->setVLinkID(src, dst, id);
	}
}

int main(int argc, char** argv) {
	reading_utilities readLine;
	string rootFile = argv[1];
	string tempStr = rootFile + ".path";

	protectedPathsOfVN_C* VNPaths = new protectedPathsOfVN_C(tempStr, 4, 5);
	setVLinkIDsInProtectedPathsOfVN(readLine, rootFile, VNPaths);
	VNPaths->printProtectedPathsOfVN(argv[2]);
	VNPaths->printStats();

	return 0;
}
