// File Created by Rana Mubeen
// In March 2019
#ifndef HUA_DISJOINT_PATHS_H
#define HUA_DISJOINT_PATHS_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

struct path_S {
	string vnode_id_src;
	string vnode_id_dst;
	string src;
	string dst;
	int id;
	int dist;
	int hopes;
	vector<string> links;
};

//struct protectedPath_S {
//	double priority;
//	vector<int> disjointPaths;
//};

struct protectedPathsOfPair_S {
	int vLinkID;
	string vnode_id_src;
	string vnode_id_dst;
	string src;
	string dst;
	vector<path_S> paths;
	vector<pair<vector<int>, double>> protectedPaths;
	vector<vector<vector<int>>> protectedPathsSet;
};

class protectedPathsOfVN_C {
private:
	vector<protectedPathsOfPair_S> protectedPathsOfVN;
	int maxSizeOfProtectedPath;
	int kOfProtectedPaths;

	void initializer(string filename);
	bool validateInputLine(string line);
	int strToInt(string str);
	path_S parseInputLine(string line);
	bool isDisjoint(path_S one, path_S two);
	vector<vector<int>> makeSizedDisjointSets(int size, int id, vector<vector<int>> initSet, vector<vector<int>> disjointTable);
	double calculatePriority(vector<int> disjointPaths, vector<path_S> paths);
	template <class T>
	vector<vector<T>> subsets(vector<T> set);
	bool isInVector(int a, vector<int> b);
	bool areEqual(vector<int> one, vector<int> two);
	vector<vector<int>> removeRepitionAndSingles(vector<vector<int>> set);
	vector<pair<vector<int>, double>> findProtectedPaths(vector<path_S> paths);
	vector<vector<vector<int>>> findProtectedPathsSet(vector<pair<vector<int>, double>> protectedPaths);
	protectedPathsOfPair_S populateFieldsOfPairPaths(string src, string dst, vector<path_S> paths);
	void printProtectedPathsOfPair(protectedPathsOfPair_S paths, ofstream& outfile);
	void printPath(path_S path, ofstream& outfile);
	void printVector(vector<int> vec, ofstream& outfile);

public:
	protectedPathsOfVN_C(string filename, int kOfProtectedPaths, int maxSizeOfDisjointSet);
	void printProtectedPathsOfVN(string filename);
	void printStats();
	void setVLinkID(string vNodeSrc, string vNodeDst, int vLinkID);
	protectedPathsOfPair_S getPathsOfVLink(int id);
};

#endif
