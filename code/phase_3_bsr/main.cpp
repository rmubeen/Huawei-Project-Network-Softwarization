#include "hua_disjoint_paths.cpp"

int main(int argc, char** argv) {

	protectedPathsOfVN_C temp(argv[1], 4, 5);
	temp.printProtectedPathsOfVN(argv[2]);
	temp.printStats();

	return 0;
}
