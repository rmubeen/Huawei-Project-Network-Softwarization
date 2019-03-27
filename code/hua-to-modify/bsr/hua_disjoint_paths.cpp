// File Created by Rana Mubeen
// In March 2019
#ifndef HUA_DISJOINT_PATHS_CPP
#define HUA_DISJOINT_PATHS_CPP

#include "hua_disjoint_paths.h"

bool protectedPathsOfVN_C::validateInputLine(string line){
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
int protectedPathsOfVN_C::strToInt(string str){
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
path_S protectedPathsOfVN_C::parseInputLine(string line){
	int length = line.length();
	path_S thisLine;
	int spaces = 0;
	string temp = "";
	string link = "";

	for(int i=0; i < length; i++){

		if(line[i] != ' '){
			temp += line[i];
		}

		if((line[i] == ' ') || i == (length-1)) {

			if (spaces == 0)
				thisLine.vnode_id_src = temp;
			else if (spaces == 1)
				thisLine.vnode_id_dst = temp;
			else if (spaces == 2)
				thisLine.src = temp;
			else if (spaces == 3)
				thisLine.dst = temp;
			else if (spaces == 4)
				thisLine.id = strToInt(temp);
			else if (spaces == 6)
				thisLine.dist = strToInt(temp);
			else if (spaces == 5)
				thisLine.hopes = strToInt(temp);
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

string protectedPathsOfVN_C::reverseLink(string link){
	string revLink = "";
	string temp = "";
	for(int i = 0; i <link.size(); i++){

		if(link[i] == '-'){
			link.erase(link.begin(), link.begin()+i+1);
			break;
		} else {
			temp += link[i];
		}

	}
	revLink = link + '-' + temp;
	return revLink;
}

bool protectedPathsOfVN_C::isDisjoint(path_S one, path_S two){
	bool status = true;
	for(int i = 0; i < one.links.size(); i++){
		for(int j = 0; j < two.links.size(); j++){
			if(one.links[i] == two.links[j])
				status = false;
			if(this->reverseLink(one.links[i]) == two.links[j])
				status = false;
		}
	}
	return status;
}

template <class T>
vector<vector<T>> protectedPathsOfVN_C::subsets(vector<T> set){
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

bool protectedPathsOfVN_C::isInVector(int a, vector<int> b){
	for(int i = 0; i < b.size(); i++){
		if (a == b[i])
			return true;
	}

	return false;
}

bool protectedPathsOfVN_C::areEqual(vector<int> one, vector<int> two){
	if(one.size() != two.size())
		return false;

	bool status = true;
	int size = one.size();

	for(int i = 0; i < size; i++){
		status = status && isInVector(one[i], two);
	}

	return status;
}

vector<vector<int>> protectedPathsOfVN_C::removeRepitionAndSingles(vector<vector<int>> set){
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

vector<vector<int>> protectedPathsOfVN_C::makeSizedDisjointSets(int size, int id, vector<vector<int>> initSet, vector<vector<int>> disjointTable){
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

double protectedPathsOfVN_C::calculatePriority(vector<int> disjointPaths, vector<path_S> paths){
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

vector<pair<vector<int>, double>> protectedPathsOfVN_C::findProtectedPaths(vector<path_S> paths){

	vector<pair<vector<int>, double>> result;

//	vector<pathS> pathVec = pathsVar.paths;
//	vector<vector<int>> disjointPaths;
	vector<vector<int>> temp2DVec;
	int size = paths.size();

//	cout << "making disjoint table" << endl;
	vector<vector<int>> disjointTable(size);
	for(int i =0; i < size; i++){
		disjointTable[i].resize(size);
	}
	for(int i =0; i < size; i++){
		for(int j =0; j < size; j++){
			if(isDisjoint(paths[i], paths[j])){
				disjointTable[i][j] = 1;
				disjointTable[j][i] = 1;
			} else {
				disjointTable[i][j] = 0;
				disjointTable[j][i] = 0;
			}
		}
	}

//	cout << "making disjoint set" << endl;
	vector<int> newProtectedPath;
	vector<vector<int>> newProtectedPaths;
	for (int i = 0; i < size; i++)
	{
		newProtectedPath.push_back(i);
		newProtectedPaths.push_back(newProtectedPath);
		newProtectedPath.clear();
	}

	for (int i = 2; i < size+1; i++)
	{
		for (int j = 0; j < size; j++)
		{
			temp2DVec = makeSizedDisjointSets(i, j, newProtectedPaths, disjointTable);
			newProtectedPaths.insert(newProtectedPaths.end(), temp2DVec.begin(), temp2DVec.end());
			temp2DVec.clear();
		}
	}

	newProtectedPaths = removeRepitionAndSingles(newProtectedPaths);

	for (int i = 0; i < newProtectedPaths.size(); i++)
	{
//		result.insert(makepair())
		result.push_back(make_pair(newProtectedPaths[i], calculatePriority(newProtectedPaths[i], paths)));
	}

	sort(result.begin(), result.end(), [](pair<vector<int>, double> first, pair<vector<int>, double> sec){return (first.second < sec.second);});

	return result;
}

vector<vector<vector<int>>> protectedPathsOfVN_C::findProtectedPathsSet(vector<pair<vector<int>, double>> protectedPaths){
	vector<vector<vector<int>>> result = {};
	int size;
	int count[this->maxSizeOfProtectedPath-1];
	for (int i = 0; i < this->maxSizeOfProtectedPath-1; i++)
		count[i] = 0;
	int cond = 0;
	bool status;
	vector<vector<int>> newProtectedPathsSet = {};
//	cout << "size in at FPDPS " << size << endl;

//cout << "GP8 " << protectedPaths.size() << endl;
	for (int i = 0; i < protectedPaths.size(); i++)
	{
		status = true;
//cout << "GP9" << endl;
		for (int j = 0; j < this->maxSizeOfProtectedPath-1; j++){
//			cout << j << "\t:j:\t" << count[j] << endl;
//cout << "GP10" << endl;
			if (count[j] != this->kOfProtectedPaths)
				status = false;
//cout << "GP11" << endl;
		}
		if(status)
			break;

//cout << "GP12" << endl;

		size = protectedPaths[i].first.size();
//		cout << size << endl;
		if((size < this->maxSizeOfProtectedPath) && (count[size-2] < this->kOfProtectedPaths)){
			count[size-2] += 1;;
			newProtectedPathsSet.push_back(protectedPaths[i].first);
//			ofstream outfile;
//			outfile.open("temp");
//			printVector(protectedPaths[i].first, outfile);
//			outfile.close();
		}
	}
//	temp.insert(temp.begin(), paths.disjointPaths.begin(), paths.disjointPaths.begin()+size);
//	cout << temp.size() << " before subset\n";
//	cout << newProtectedPathsSet.size() << endl;
	if(newProtectedPathsSet.size() != 0)
		result = subsets(newProtectedPathsSet);

	return result;
}

protectedPathsOfPair_S protectedPathsOfVN_C::populateFieldsOfPairPaths(string src, string dst, vector<path_S> paths){
	protectedPathsOfPair_S newPairPaths;
	newPairPaths.src = src;
	newPairPaths.dst = dst;
	newPairPaths.paths = paths;
	newPairPaths.vnode_id_src = newPairPaths.paths[0].vnode_id_src;
	newPairPaths.vnode_id_dst = newPairPaths.paths[0].vnode_id_dst;
	newPairPaths.vLinkID = -1;
//cout << "GP5" << endl;

	newPairPaths.protectedPaths = findProtectedPaths(paths);
//cout << "GP6" << endl;
	newPairPaths.protectedPathsSet = findProtectedPathsSet(newPairPaths.protectedPaths);
//cout << "GP7" << endl;

	return newPairPaths;
}

void protectedPathsOfVN_C::initializer(string filename){
	ifstream infile;
	infile.open(filename);
	if(!infile){
		cout << "ERR: error opening input file " << filename << endl;
	}

	path_S newPath;
	vector<path_S> newPaths;
	string src = "";
	string dst = "";
	string line;
//int coutvar = 0;
	while(getline(infile, line)) {
		if (validateInputLine(line)){
			newPath = parseInputLine(line);
			if (src == ""){
				src = newPath.src;
				dst = newPath.dst;
			}
//cout << coutvar << endl;
//coutvar++;
			if (src == newPath.src && dst == newPath.dst){
				newPaths.push_back(newPath);
			} else {
//cout << "GP1" << endl;

				sort(newPaths.begin(), newPaths.end(), [](path_S first, path_S second){return (first.id < second.id);});
//cout << "GP2" << endl;
				this->protectedPathsOfVN.push_back(populateFieldsOfPairPaths(src, dst, newPaths));
//cout << "GP3" << endl;
				src = newPath.src;
				dst = newPath.dst;
				newPaths.clear();
				newPaths.push_back(newPath);
//cout << "GP4" << endl;
			}
		}
	}
	this->protectedPathsOfVN.push_back(populateFieldsOfPairPaths(src, dst, newPaths));

	infile.close();
}

protectedPathsOfVN_C::protectedPathsOfVN_C(string filename, int kOfProtectedPaths, int maxSizeOfDisjointSet){
	this->kOfProtectedPaths = kOfProtectedPaths;
	this->maxSizeOfProtectedPath = maxSizeOfDisjointSet;
	initializer(filename);

}

void protectedPathsOfVN_C::printVector(vector<int> vec, ofstream& outfile){
	outfile << "{";
	for(int i = 0; i < vec.size(); i++){
		outfile << vec[i];
		if (i != vec.size()-1)
			outfile << " ";
	}
	outfile << "}";
}

void protectedPathsOfVN_C::printPath(path_S path, ofstream& outfile){
	outfile << "PATH ID: \t" << path.id;
	outfile << "\tDISTANCE: \t" << path.dist;
	outfile << "\tHOPES: \t" << path.hopes;
	outfile << "\tLINKS: ";
	for(int i = 0; i < path.links.size(); i++){
		outfile << "{" << path.links[i] << "} ";
	}
	outfile << endl;
}

void protectedPathsOfVN_C::printProtectedPathsOfPair(protectedPathsOfPair_S paths, ofstream& outfile){
	outfile << "V LINK ID: \t" << paths.vLinkID << endl;
	outfile << "V SOURCE: \t" << paths.vnode_id_src << endl;
	outfile << "V DESTINATION: \t" << paths.vnode_id_dst << endl;
	outfile << "SOURCE: \t" << paths.src << endl;
	outfile << "DESTINATION: \t" << paths.dst << endl;
	outfile << "PATHS: " << paths.paths.size() << endl;
	outfile << "----------------------------------------------------------" << endl;
	for(int i = 0; i < paths.paths.size(); i++){
		printPath(paths.paths[i], outfile);
	}
	outfile << "----------------------------------------------------------" << endl;
	outfile << "DISJOINT SETS: " << paths.protectedPaths.size() << endl;
	for(int i = 0; i < paths.protectedPaths.size(); i++){
		outfile << " " << paths.protectedPaths[i].second << " ";
		printVector(paths.protectedPaths[i].first, outfile);
		outfile << endl;
	}
	outfile << "----------------------------------------------------------" << endl;
	outfile << "SETS OF DISJOINT SETS: " << paths.protectedPathsSet.size() << endl;
	for(int i = 0; i < paths.protectedPathsSet.size(); i++){
		outfile << i << endl;
		for (int j = 0; j < paths.protectedPathsSet[i].size(); j++)
		{
			printVector(paths.protectedPathsSet[i][j], outfile);
			outfile << " "; //<< pathsVar.disjointPathsSet[i][j].priority << " ";
		}
			outfile << endl;
	}
	outfile << "----------------------------------------------------------" << endl;
}

void protectedPathsOfVN_C::printProtectedPathsOfVN(string filename){
	int size = this->protectedPathsOfVN.size();
	ofstream outfile;
	outfile.open(filename);

	for (int i = 0; i < size; i++) {
		printProtectedPathsOfPair(this->protectedPathsOfVN[i], outfile);
	}

	outfile.close();
}

void protectedPathsOfVN_C::printStats(){
	int totalProtectedPaths = 0;
	int totalProtectedPathsSet = 0;
	vector<int> protectedPathsStats(this->protectedPathsOfVN.size());
	vector<int> protectedPathsSetStats(this->protectedPathsOfVN.size());

	for(int i = 0; i < this->protectedPathsOfVN.size(); i++){
		protectedPathsStats[i] = this->protectedPathsOfVN[i].protectedPaths.size();
		totalProtectedPaths += this->protectedPathsOfVN[i].protectedPaths.size();
		protectedPathsSetStats[i] = this->protectedPathsOfVN[i].protectedPathsSet.size();
		totalProtectedPathsSet += this->protectedPathsOfVN[i].protectedPathsSet.size();

		cout << this->protectedPathsOfVN[i].src;
		cout << " \t" << this->protectedPathsOfVN[i].dst;
		cout << " \t" << this->protectedPathsOfVN[i].paths.size();
		cout << " \t" << this->protectedPathsOfVN[i].protectedPaths.size();
		cout << " \t" << this->protectedPathsOfVN[i].protectedPathsSet.size() << endl;
	}

	sort(protectedPathsStats.begin(), protectedPathsStats.end());
	sort(protectedPathsSetStats.begin(), protectedPathsSetStats.end());

	cout << "Protected Paths\n";
	cout << "Min: \t\t" << protectedPathsStats[0] << endl;
	cout << "Max: \t\t" << protectedPathsStats[protectedPathsStats.size()-1] << endl;
	cout << "Median: \t" << protectedPathsStats[protectedPathsStats.size()/2] << endl;
	cout << "Mean: \t\t" << totalProtectedPaths/(protectedPathsStats.size()) << endl;

	cout << "Protected Paths Set\n";
	cout << "Min: \t\t" << protectedPathsSetStats[0] << endl;
	cout << "Max: \t\t" << protectedPathsSetStats[protectedPathsSetStats.size()-1] << endl;
	cout << "Median: \t" << protectedPathsSetStats[protectedPathsSetStats.size()/2] << endl;
	cout << "Mean: \t\t" << totalProtectedPathsSet/(protectedPathsSetStats.size()) << endl;
}

void protectedPathsOfVN_C::setVLinkID(string vNodeSrc, string vNodeDst, int vLinkID){
	for(int i = 0; i < this->protectedPathsOfVN.size(); i++) {
		if((this->protectedPathsOfVN[i].vnode_id_src == vNodeSrc) && (this->protectedPathsOfVN[i].vnode_id_dst == vNodeDst)){
			this->protectedPathsOfVN[i].vLinkID = vLinkID;
		}
	}
}

protectedPathsOfPair_S protectedPathsOfVN_C::getPathsOfVLink(int id){
	for(int i = 0; i < this->protectedPathsOfVN.size(); i++) {
		if(this->protectedPathsOfVN[i].vLinkID == id){
			return protectedPathsOfVN[i];
		}
	}
}

#endif
