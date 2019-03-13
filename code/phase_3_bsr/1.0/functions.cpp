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
	vector<vector<path>> disjointPaths;
};