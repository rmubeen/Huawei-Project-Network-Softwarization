#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct path {
	string src;
	string dst;
	int id;
	int dist;
	int hopes;
	vector<string> links;
};

struct paths {
	string src;
	string dst;
	vector<path> pathVec;
	vector<vector<int>> disjointPaths;
};

bool validateLine(string line){
	int length = line.length();
	int spaces = 0;
	bool status = true;
	int temp;

	for(int i=0; i < length; i++){
		if (line[i] == ' ')
			spaces++;
		else {
			temp = int(line[i]);
			if (temp > 57 || temp < 48)
				status = false;
		}
	}

	if (spaces < 6 || spaces % 2 != 0)
		status = false;

	return status;
}

int getNumber(string str){
	int number = 0;

	for(int i=0; i < str.length(); i++){
		number = (number * 10) + (int(str[i]) - 48);
	}

	return number;
}

path parseLine(string line){
	int length = line.length();
	path thisLine;
	int spaces = 0;
	string temp = "";
	string link = "";

	for(int i=0; i < length; i++){

		if(line[i] != ' '){
			temp += line[i];
		}

		if(line[i] == ' ' || i == (length-1)) {

			if (spaces == 0)
				thisLine.src = temp;
			else if (spaces == 1)
				thisLine.dst = temp;
			else if (spaces == 2)
				thisLine.id = getNumber(temp);
			else if (spaces == 3)
				thisLine.dist = getNumber(temp);
			else if (spaces == 4)
				thisLine.hopes = getNumber(temp);
			else {

				if(link == "")
					link = temp;
				else {
					link += '-';
					link += temp;
					thisLine.links.push_back(link);
					link = "";
				}

			}

			temp = "";
			spaces++;
		}
	}

	return thisLine;
}

void printVector(vector<int> vec, ofstream& outfile){
	outfile << "{";
	for(int i = 0; i < vec.size(); i++){
		outfile << vec[i];
		if (i != vec.size()-1)
			outfile << " ";
	}
	outfile << "}";
}

void printPath(path pathVar, ofstream& outfile){
	outfile << "PATH ID: \t" << pathVar.id;
	outfile << "\tDISTANCE: \t" << pathVar.dist;
	outfile << "\tHOPES: \t" << pathVar.hopes;
	outfile << "\tLINKS: ";
	for(int i = 0; i < pathVar.links.size(); i++){
		outfile << "{" << pathVar.links[i] << "} ";
	}
	outfile << endl;
}

paths populatePathsVar(string src, string dst, vector<path> pathVec){
	paths temp;
	temp.src = src;
	temp.dst = dst;
	temp.pathVec = pathVec;
	return temp;
}

void printPaths(paths pathsVar, ofstream& outfile){
	outfile << "SOURCE: \t" << pathsVar.src << endl;
	outfile << "DESTINATION: \t" << pathsVar.dst << endl;
	outfile << "PATHS: " << pathsVar.pathVec.size() << endl;
//	outfile << "----------------------------------------------------------" << endl;
//	for(int i = 0; i < pathsVar.pathVec.size(); i++){
//		printPath(pathsVar.pathVec[i], outfile);
//	}
//	outfile << "----------------------------------------------------------" << endl;
	outfile << "DISJOINT SETS: " << pathsVar.disjointPaths.size() << endl;	
	for(int i = 0; i < pathsVar.disjointPaths.size(); i++){
		printVector(pathsVar.disjointPaths[i], outfile);
		outfile << endl;
	}
	outfile << "----------------------------------------------------------" << endl;
}

bool isDisjoint(path one, path two){
	bool status = true;
	for(int i = 0; i < one.links.size(); i++){
		for(int j = 0; j < two.links.size(); j++){
			if(one.links[i] == two.links[j])
				status = false;
		}
	}
	return status;
}

vector<vector<int>> subsets(vector<int> set){
	int size = set.size();
	vector<vector<int>> subSets;
	vector<vector<int>> newSubSets;
	vector<int> subSet;
	int path = set[0];

	if(size == 1){
		subSet.push_back(path);
		subSets.push_back(subSet);
	} else {
		set.erase(set.begin());
		subSets = subsets(set);
		size = subSets.size();

		subSet.push_back(path);
		subSets.push_back(subSet);

		for(int i = 0; i < size; i++){
			subSet = subSets[i];
			subSet.push_back(path);
			subSets.push_back(subSet);
		}
	}
	return subSets;
}

bool isInVector(int a, vector<int> b){
	for(int i = 0; i < b.size(); i++){
		if (a == b[i])
			return true;
	}

	return false;
}

bool areEqual(vector<int> one, vector<int> two){
	if(one.size() != two.size())
		return false;
	
	bool status = true;
	int size = one.size();

	for(int i = 0; i < size; i++){
		status = status && isInVector(one[i], two);
	}

	return status;
}

vector<vector<int>> removeRepitionAndSingles(vector<vector<int>> set){
	int size = set.size();
	int i = 0;

	while(i < size){
		if(set[i].size() < 2){
			set.erase(set.begin()+i);
			i--;
			size--;
		} else {
			for(int j = i+1; j < size; j++){
				if (areEqual(set[i], set[j])){
					set.erase(set.begin()+i);
					i--;
					size--;
					break;
				}
			}
		}

		i++;
	}
	return set;
}

vector<vector<int>> findDisjointPaths(paths pathsVar){
	int totPaths = pathsVar.pathVec.size();
	vector<vector<int>> disjointSet;
	vector<vector<int>> subSets;
	vector<int> temp = {};
	bool status = true;

	for(int i = 0; i < totPaths; i++){
		temp.push_back(pathsVar.pathVec[i].id);

		for(int j = 0; j < totPaths; j++){
			if(isDisjoint(pathsVar.pathVec[i], pathsVar.pathVec[j])){
				status = true;
				for (int l = 1; l < temp.size(); l++) {
					status = status && (isDisjoint(pathsVar.pathVec[j], pathsVar.pathVec[temp[l]]));
				}
				if(status)
					temp.push_back(pathsVar.pathVec[j].id);
			}
		}

		sort(temp.begin(), temp.end());
		subSets = subsets(temp);
		subSets.push_back(temp);
		for(int k = 0; k < subSets.size(); k++){
			sort(subSets[k].begin(), subSets[k].end());
			disjointSet.push_back(subSets[k]);
		}
		temp.clear();
	}

	disjointSet = removeRepitionAndSingles(disjointSet);
	sort(disjointSet.begin(), disjointSet.end());

	return disjointSet;
}

int main(int argc, char** argv) {
	ifstream infile;
	string line;
	path tempPath;
  	vector<path> pathVec;
  	vector<paths> pathsVec;
  	string src = "";
  	string dst = "";

	infile.open(argv[1]);
	if(!infile) {
		cout << "Cannot open input file.\n";
    	return 1;
  	}

	while(getline(infile, line)) {
		if (validateLine(line)){
			tempPath = parseLine(line);
			if (src == ""){
				src = tempPath.src;
				dst = tempPath.dst;
			}
			if (src == tempPath.src && dst == tempPath.dst){
				pathVec.push_back(tempPath);
			} else {
				pathsVec.push_back(populatePathsVar(src, dst, pathVec));
				src = tempPath.src;
				dst = tempPath.dst;
				pathVec.clear();
				pathVec.push_back(tempPath);
			}
		}
	}
	pathsVec.push_back(populatePathsVar(src, dst, pathVec));
	infile.close();

	ofstream outfile;
	outfile.open(argv[2]);
	if(!outfile){
		cout << "Cannot open output file.\n";
		return 1;
	}
	for(int i = 0; i < pathsVec.size(); i++){
		pathsVec[i].disjointPaths = findDisjointPaths(pathsVec[i]);
		printPaths(pathsVec[i], outfile);
	}
	outfile.close();

	return 0;
}