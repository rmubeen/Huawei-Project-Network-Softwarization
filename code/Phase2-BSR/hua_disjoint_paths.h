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

/*
 * This structure stores a virtual path in VN topology.
 */
struct path_S {
	string vnode_id_src; // vnode src id
	string vnode_id_dst; // vnode dst id
	string src; // src id
	string dst; // dst id
	int id; // path id
	int dist; // path length
	int hopes; // number of hopes in path
	vector<string> links; // path links
};

/*
 * This structure stores all paths for a vLink.
 */
struct protectedPathsOfPair_S {
	int vLinkID; // vLink id
	string vnode_id_src; // vnode src id
	string vnode_id_dst; // vnode dst id
	string src; // src id
	string dst; // dst id
	vector<path_S> paths; // all vLink paths
	vector<pair<vector<int>, double>> protectedPaths; // all vlink protected paths with average path length where each protected path consists of mutually disjoint virtual paths
	vector<vector<vector<int>>> protectedPathsSet; // all subsets of protected paths set
};

/*
 * This class takes VN paths as input file, parse the virtual paths and computes protected paths set.
 */
class protectedPathsOfVN_C {
private:
	vector<protectedPathsOfPair_S> protectedPathsOfVN; // protected paths for all vLinks.
	int maxSizeOfProtectedPath; // upper limit of number of virtual paths in protected path for making protected paths set.
	int kOfProtectedPaths; // number of protected paths to choose from each size of protected path from lower limit: 2 to upper limit: maxSizeOfDisjointSet

	/*
	 * It reads input data file.
	 */
	void initializer(string filename);

	/*
	 * It takes a line from input file and returns true of it stores a valid virtual path but returns false if it does not.
	 */
	bool validateInputLine(string line);

	/*
	 * It converts a string to an integer.
	 */
	int strToInt(string str);

	/*
	 * It takes a line from input file, parses it, and returns path_S object.
	 */
	path_S parseInputLine(string line);

	/*
	 * It takes two paths as input and returns true if they're disjoint but returns false if they are not.
	 */
	bool isDisjoint(path_S one, path_S two);

	/*
	 * It returns all disjoint sets of given size from disjoint table.
	 */
	vector<vector<int>> makeSizedDisjointSets(int size, int id, vector<vector<int>> initSet, vector<vector<int>> disjointTable);

	/*
	 * It takes all virtual paths of a vLink and a protected path. And, then, returns the average path length of the protected path.
	 */
	double calculatePriority(vector<int> disjointPaths, vector<path_S> paths);

	/*
	 * It takes a vector as input and returns all subsets of that set.
	 */
	template <class T>
	vector<vector<T>> subsets(vector<T> set);

	/*
	 * It returns true if given integer "a" exists in the vector "b" but false it it does not..
	 */
	bool isInVector(int a, vector<int> b);

	/*
	 * It compares two input vectors and returns true if they are equal but false if they are not.
	 */
	bool areEqual(vector<int> one, vector<int> two);

	/*
	 * It removes all repititions of a vector in a given 2-D vector and also removes 1-D vectors with only one element in it.
	 */
	vector<vector<int>> removeRepitionAndSingles(vector<vector<int>> set);

	/*
	 * It takes virtual paths of a vLink and returns all sets of mutually disjoint virtual paths.
	 */
	vector<pair<vector<int>, double>> findProtectedPaths(vector<path_S> paths);

	/*
	 * It takes protectedPaths set of a vLink and returns all the subsets.
	 */
	vector<vector<vector<int>>> findProtectedPathsSet(vector<pair<vector<int>, double>> protectedPaths);

	/*
	 * It takes src, dst and paths of a vLink, and then, computes/populates all the protected paths of that vLink.
	 */
	protectedPathsOfPair_S populateFieldsOfPairPaths(string src, string dst, vector<path_S> paths);

	/*
	 * It prints protected paths of all vLinks to the given output file object.
	 */
	void printProtectedPathsOfPair(protectedPathsOfPair_S paths, ofstream& outfile);

	/*
	 * It prints a virtual path to the given output file object.
	 */
	void printPath(path_S path, ofstream& outfile);

	/*
	 * It prints a given 1-D vector to the given output file object.
	 */
	void printVector(vector<int> vec, ofstream& outfile);

	/*
	 * It reverses src and dst of a given path hope.
	 */
	string reverseLink(string link);

public:
	/*
	 * It takes input filename, and tuning parameters to compute protected paths.
	 */
	protectedPathsOfVN_C(string filename, int kOfProtectedPaths, int maxSizeOfDisjointSet);

	/*
	 * It prints VN's proteted paths in an output file with given filename.
	 */
	void printProtectedPathsOfVN(string filename);

	/*
	 * It prints different stats for each vLink like total number of paths, average paths length etc.
	 */
	void printStats();

	/*
	 * It takes vNode src/dst ids and vLink ID, then, sets those values.
	 */
	void setVLinkID(string vNodeSrc, string vNodeDst, int vLinkID);

	/*
	 * It returns protected paths for given vLink ID.
	 */
	protectedPathsOfPair_S getPathsOfVLink(int id);
};

#endif
