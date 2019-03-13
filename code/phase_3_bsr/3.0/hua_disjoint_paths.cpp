// File Created by Rana Mubeen
// In March 2019
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "typeDef.cpp"
using namespace std;

#define topDisjointPathsSet 15

// Checks if all lines in the path file are valid:
//		have enough information and don't have any invalid character.
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
			if ((temp > 57 || temp < 48) && (line[i] != '.'))
				status = false;
		}
	}

	if (spaces < 6 || spaces % 2 != 0)
		status = false;

	return status;
}

// Converts a string into a number
int getNumber(string str){
	int number = 0;

	for(int i=0; i < str.length(); i++){
		if(str[i] == '.')
			break;
		else
			number = (number * 10) + (int(str[i]) - 48);
	}

	return number;
}

// Parses a line(path) in path file and stores appropriate information in path struct.
pathS parseLine(string line){
	int length = line.length();
	pathS thisLine;
	int spaces = 0;
	string temp = "";
	string link = "";

	for(int i=0; i < length; i++){

		if(line[i] != ' '){
			temp += line[i];
		}

		if((line[i] == ' ') || i == (length-1)) {

			if (spaces == 0)
				thisLine.vnod_id_1 = temp;
			else if (spaces == 1)
				thisLine.vnod_id_2 = temp;
			else if (spaces == 2)
				thisLine.src = temp;
			else if (spaces == 3)
				thisLine.dst = temp;
			else if (spaces == 4)
				thisLine.id = getNumber(temp);
			else if (spaces == 6)
				thisLine.dist = getNumber(temp);
			else if (spaces == 5)
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

// Prints a vector in output file.
void printVector(vector<int> vec, ofstream& outfile){
	outfile << "{";
	for(int i = 0; i < vec.size(); i++){
		outfile << vec[i];
		if (i != vec.size()-1)
			outfile << " ";
	}
	outfile << "}";
}


// Prin
void printPath(pathS pathVar, ofstream& outfile){
	outfile << "PATH ID: \t" << pathVar.id;
	outfile << "\tDISTANCE: \t" << pathVar.dist;
	outfile << "\tHOPES: \t" << pathVar.hopes;
	outfile << "\tLINKS: ";
	for(int i = 0; i < pathVar.links.size(); i++){
		outfile << "{" << pathVar.links[i] << "} ";
	}
	outfile << endl;
}

allPathsS populateAllPathsVar(string src, string dst, vector<pathS> pathVec){
	allPathsS temp;
	temp.src = src;
	temp.dst = dst;
	temp.paths = pathVec;
	return temp;
}

void printPaths(allPathsS pathsVar, ofstream& outfile){
	outfile << "SOURCE: \t" << pathsVar.src << endl;
	outfile << "DESTINATION: \t" << pathsVar.dst << endl;
	outfile << "PATHS: " << pathsVar.paths.size() << endl;
	outfile << "----------------------------------------------------------" << endl;
	for(int i = 0; i < pathsVar.paths.size(); i++){
		printPath(pathsVar.paths[i], outfile);
	}
	outfile << "----------------------------------------------------------" << endl;
	outfile << "DISJOINT SETS: " << pathsVar.disjointPaths.size() << endl;	
	for(int i = 0; i < pathsVar.disjointPaths.size(); i++){
		outfile << " " << pathsVar.disjointPaths[i].priority << " ";
		printVector(pathsVar.disjointPaths[i].disjointPaths, outfile);
		outfile << endl;
	}
/*	outfile << "----------------------------------------------------------" << endl;
	outfile << "SETS OF DISJOINT SETS: " << pathsVar.disjointPathsSet.size() << endl;	
	for(int i = 0; i < pathsVar.disjointPathsSet.size(); i++){
		outfile << i << endl;
		for (int j = 0; j < pathsVar.disjointPathsSet[i].size(); j++)
		{
			printVector(pathsVar.disjointPathsSet[i][j].disjointPaths, outfile);
			outfile << " "; //<< pathsVar.disjointPathsSet[i][j].priority << " ";
		}
			outfile << endl;
	}*/
	outfile << "----------------------------------------------------------" << endl;
}

bool isDisjoint(pathS one, pathS two){
	bool status = true;
	for(int i = 0; i < one.links.size(); i++){
		for(int j = 0; j < two.links.size(); j++){
			if(one.links[i] == two.links[j])
				status = false;
		}
	}
	return status;
}

template <class T>
vector<vector<T>> subsets(vector<T> set){
	int size = set.size();
	vector<vector<T>> subSets;
	vector<vector<T>> newSubSets;
	vector<T> subSet;
	T path = set[0];

	if(size == 1){
		subSet.push_back(path);
		subSets.push_back(subSet);
//		cout << size << " U\n";
	} else {
//		cout << size << " D\n";
		set.erase(set.begin());
		subSets = subsets(set);
		size = subSets.size();

		subSet.push_back(path);
		subSets.push_back(subSet);

//		cout << size << " D\n";
		for(int i = 0; i < size; i++){
			subSet = subSets[i];
			subSet.push_back(path);
			subSets.push_back(subSet);
		}
	}
//	cout << size << " FD\n";
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

vector<vector<int>> makeSizedDisjointSets(int size, int id, vector<vector<int>> initSet, vector<vector<int>> disjointTable){
	vector<vector<int>> out;
	int setSize = initSet.size();
	bool status;

	// takes every present set
	for (int i = 0; i < setSize; i++)
	{
		//only takes sets with size one less than the size required
		if (initSet[i].size() == size-1)
		{
			status = true;
			for (int j = 0; j < size-1; j++)
			{
				if(disjointTable[id][initSet[i][j]] == 0){
					status = false;
					break;
				}// else
//					cout << id << " " << j << " " << status << " " << disjointTable[id][initSet[i][j]] << endl;
			}
			if(status){
				initSet[i].push_back(id);
				out.push_back(initSet[i]);
			}
		}
	}

	return out;
}

double calculatePriority(vector<int> disjointPaths, vector<pathS> paths){
	int totalDist = 0;
	int n = disjointPaths.size();
	double priority;

	for (int i = 0; i < n; i++)
	{
		totalDist += paths[disjointPaths[i]].dist;
	}

	priority = (double)totalDist/(n * (n-1));

	return priority;
}

bool pathsSortComp(pathS first, pathS second){
	return (first.id < second.id);
}

bool disjointPathsSortComp(disjointPathsS first, disjointPathsS second){
	return (first.priority < second.priority);
}

vector<disjointPathsS> findDisjointPaths(allPathsS pathsVar){
	vector<pathS> pathVec = pathsVar.paths;
	vector<vector<int>> disjointPaths;
	vector<vector<int>> temp2DVec;
	vector<int> tempVec;
	int size = pathVec.size();

//	cout << "making disjoint table" << endl;
	vector<vector<int>> disjointTable(size);
	for(int i =0; i < size; i++){
		disjointTable[i].resize(size);
	}
	for(int i =0; i < size; i++){
		for(int j =0; j < size; j++){
			if(isDisjoint(pathVec[i], pathVec[j])){
//				cout << i << " Ye " << j <<"\n";
				disjointTable[i][j] = 1;
				disjointTable[j][i] = 1;
			} else {
//				cout << i << " Naaaaaaaaaaaaaaaaaa " << j << "\n";
				disjointTable[i][j] = 0;
				disjointTable[j][i] = 0;				
			}
		}
	}

//	cout << "making disjoint set" << endl;
	for (int i = 0; i < size; i++)
	{
		tempVec.push_back(i);
		disjointPaths.push_back(tempVec);
		tempVec.clear();
	}

	for (int i = 2; i < size+1; i++)
	{
		for (int j = 0; j < size; j++)
		{
			temp2DVec = makeSizedDisjointSets(i, j, disjointPaths, disjointTable);
			disjointPaths.insert(disjointPaths.end(), temp2DVec.begin(), temp2DVec.end());
			temp2DVec.clear();
		}
	}

	disjointPaths = removeRepitionAndSingles(disjointPaths);

	vector<disjointPathsS> disjointPathsWithPr;
	disjointPathsS temp;
	for (int i = 0; i < disjointPaths.size(); i++)
	{
		temp.disjointPaths = disjointPaths[i];
		temp.priority = calculatePriority(disjointPaths[i], pathsVar.paths);
		disjointPathsWithPr.push_back(temp);
	}

	sort(disjointPathsWithPr.begin(), disjointPathsWithPr.end(), disjointPathsSortComp);

	return disjointPathsWithPr;
}

vector<vector<disjointPathsS>> populateDisjointPathSets(allPathsS paths){
	vector<vector<disjointPathsS>> result;
	int size = paths.disjointPaths.size();
	size = min(topDisjointPathsSet, size);
//	cout << "size in at FPDPS " << size << endl;
	vector<disjointPathsS> temp;
	temp.insert(temp.begin(), paths.disjointPaths.begin(), paths.disjointPaths.begin()+size);
//	cout << temp.size() << " before subset\n";
	result = subsets(temp);
//	cout << temp.size() << " after subset\n";

	// remove singles
/*	size = result.size();
	int i = 0;
	int increment;
	while(i < size){
		increment = 1;
		if(result[i].size() == 1) {
			result.erase(result.begin()+i);
			increment = 0;
			size -= 1;
		}
		i += increment;
	}*/

	return result;
}

int main(int argc, char** argv) {
	ifstream infile;
	string line;
	pathS tempPath;
  	vector<pathS> pathVec;
  	vector<allPathsS> pathsVec;
  	string src = "";
  	string dst = "";

//	cout << "CP1\n";
	infile.open(argv[1]);
	if(!infile) {
		cout << "Cannot open input file.\n";
    	return 1;
  	}
//  	getline(infile, line);
//	cout << "CP2 " << argv[1] << " " << line << "\n";
	while(getline(infile, line)) {
		if (validateLine(line)){
			tempPath = parseLine(line);
			if (src == ""){
				src = tempPath.src;
				dst = tempPath.dst;
			}

//			cout << "CP3\n";
			if (src == tempPath.src && dst == tempPath.dst){
				pathVec.push_back(tempPath);
			} else {
				sort(pathVec.begin(), pathVec.end(), pathsSortComp);
				pathsVec.push_back(populateAllPathsVar(src, dst, pathVec));
				src = tempPath.src;
				dst = tempPath.dst;
				pathVec.clear();
				pathVec.push_back(tempPath);
			}
		}
	}
	pathsVec.push_back(populateAllPathsVar(src, dst, pathVec));
	infile.close();
//	cout << "CP4\n";

	ofstream outfile;
	outfile.open(argv[2]);
	if(!outfile){
//		cout << "Cannot open output file.\n";
		return 1;
	}
//	cout << "CP5\n";

	vector<int> stats(pathsVec.size());
	int total = 0;
	for(int i = 0; i < pathsVec.size(); i++){
//		cout << i << " \n";

		pathsVec[i].disjointPaths = findDisjointPaths(pathsVec[i]);
//		cout << "CP6\n";
		pathsVec[i].disjointPathsSet = populateDisjointPathSets(pathsVec[i]);
//		sort(pathsVec[i].disjointPaths.begin(), pathsVec[i].disjointPaths.end());
//		cout << "CP7\n";
		printPaths(pathsVec[i], outfile);
//		cout << "CP8\n";
		stats[i] = pathsVec[i].disjointPaths.size();
//		cout << "CP8\n";
		total += stats[i];
	}
	outfile.close();

	sort(stats.begin(), stats.end());
	cout << "Min: \t\t" << stats[0] << endl;
	cout << "Max: \t\t" << stats[stats.size()-1] << endl;
	cout << "Median: \t" << stats[stats.size()/2] << endl;
	cout << "Mean: \t\t" << total/(stats.size()) << endl;

	return 0;
}