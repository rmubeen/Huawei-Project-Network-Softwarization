#include <string>
#include <vector>
using namespace std;

struct pathS {
	string vnod_id_1;
	string vnod_id_2;
	string src;
	string dst;
	int id;
	int dist;
	int hopes;
	vector<string> links;
};

struct disjointPathsS {
	double priority;
	vector<int> disjointPaths;
};

struct allPathsS {
	string src;
	string dst;
	int size;
	vector<pathS> paths;
	vector<disjointPathsS> disjointPaths;
	vector<vector<disjointPathsS>> disjointPathsSet;
};

//Min: 		24
//Max: 		212
//Median: 	78
//Mean: 	82
