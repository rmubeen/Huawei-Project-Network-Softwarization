// File Created by Rana Mubeen
// In March 2019

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
				thisLine.vnod_id_1 = temp;
			else if (spaces == 1)
				thisLine.vnod_id_2 = temp;
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

bool protectedPathsOfVN_C::isDisjoint(path_S one, path_S two){
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
	vector<vector<vector<int>>> result;
	int size;
	int count[maxSizeOfProtectedPath];
	for (int i = 0; i < maxSizeOfProtectedPath; i++)
		count[i] = 0;
	int cond = 0;
	bool status;
	vector<vector<int>> newProtectedPathsSet;
//	cout << "size in at FPDPS " << size << endl;

	for (int i = 0; i < protectedPaths.size(); i++)
	{
		status = true;
		for (int j = 0; j < maxSizeOfProtectedPath; j++){
			if (count[i] != kOfProtectedPaths)
				status = false;
		}
		if(status)
			break;


		size = protectedPaths[i].first.size();
		if((size < maxSizeOfProtectedPath) && (count[size-1] < kOfProtectedPaths)){
			count[size-1] += 1;;
			newProtectedPathsSet.push_back(protectedPaths[i].first);
		}
	}
//	temp.insert(temp.begin(), paths.disjointPaths.begin(), paths.disjointPaths.begin()+size);
//	cout << temp.size() << " before subset\n";
	cout << newProtectedPathsSet.size() << endl;
	result = subsets(newProtectedPathsSet);

	return result;
}

protectedPathsOfPair_S protectedPathsOfVN_C::populateFieldsOfPairPaths(string src, string dst, vector<path_S> paths){
	protectedPathsOfPair_S newPairPaths;
	newPairPaths.src = src;
	newPairPaths.dst = dst;
	newPairPaths.paths = paths;

	newPairPaths.protectedPaths = findProtectedPaths(paths);
	newPairPaths.protectedPathsSet = findProtectedPathsSet(newPairPaths.protectedPaths);

	return newPairPaths;
}

void protectedPathsOfVN_C::initializer(string filename){
	ifstream infile;
	infile.open(filename);

	path_S newPath;
	vector<path_S> newPaths;
	string src = "";
	string dst = "";
	string line;

	while(getline(infile, line)) {
		if (validateInputLine(line)){
			newPath = parseInputLine(line);
			if (src == ""){
				src = newPath.src;
				dst = newPath.dst;
			}

			if (src == newPath.src && dst == newPath.dst){
				newPaths.push_back(newPath);
			} else {
				sort(newPaths.begin(), newPaths.end(), [](path_S first, path_S second){return (first.id < second.id);});
				this->protectedPathsOfVN.push_back(populateFieldsOfPairPaths(src, dst, newPaths));
				src = newPath.src;
				dst = newPath.dst;
				newPaths.clear();
				newPaths.push_back(newPath);
			}
		}
	}
	this->protectedPathsOfVN.push_back(populateFieldsOfPairPaths(src, dst, newPaths));

	infile.close();
}

protectedPathsOfVN_C::protectedPathsOfVN_C(string filename){

    initializer(filename);

}
