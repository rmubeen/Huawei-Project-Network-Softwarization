//
// Created by sepehr on 08/09/18.
//

#ifndef HUA_TO_MODIFY_SOLUTION_H
#define HUA_TO_MODIFY_SOLUTION_H

#include <fstream>
#include <vector>
#include "reach_table.h"
#include "basic_functions.cpp"
#include "hua_disjoint_paths.cpp"
using namespace std;

/*
 * This class stores an embedding solution.
 */
class solution {
private:
    vector<int> paths; // vector of ids of paths used in the solution.
    vector<int> slices_req; // vector of slices required on each path.
    vector<int> slices_start; // starting position of each slice embedding on the paths used.
    vector<int> tuples; // ids of tuples, used for slice allocation on each path, according to reach table.
    int needed_slices; // total number of slices needed on the VN for this solution.

    vector<int> next_demands;
    vector<vector<int>> path_degrees;

public:
  /*
   * It takes a vlid slice allocation and populates the realatd variables.
   */
    solution(vector<int> paths, vector<int> tuples, vector<int> slices_req, vector<int> starting_slices_index, int needed_slices, vector<vector<int>> path_degrees, vector<int> next_demands) {
        this->paths = paths;
        this->tuples = tuples;
        this->slices_start = starting_slices_index;
        this->slices_req = slices_req;
        this->needed_slices = needed_slices;
        this->path_degrees = path_degrees;
        this->next_demands = next_demands;
    }

    /*
     * It stores an invalid solution.
     */
    solution() {
        this->needed_slices = -1;
    }

    /*
     * It returns a vector of paths used in the solution.
     */
    vector<int> get_paths() {
        return paths;
    }

    /*
     * It returns a vector of tuple-indeces from reach table which were used in the solution.
     */
    vector<int> get_tuples() {
        return tuples;
    }

    /*
     * It returns a vector of starting indeces of slice allocations on paths which were used in the solution.
     */
    vector<int> get_starting_slices() {
        return slices_start;
    }

    /*
     * It returns a vector of required number of slices in each paths which were used in the solution.
     */
    vector<int> get_req_slices() {
        return slices_req;
    }

    /*
     * It returns total number of slices for this solution.
     * Where: needed slices on a path = number of required slices on the path * hopes on that path
     */
    int get_needed_slices() {
        return needed_slices;
    }

    /*
     * It prints solution on the standard output.
     */
    void print_solution(){
      cout<<"paths: ";
      printVector(paths);
      cout<<"slices required:\n";
      printVector(slices_req);
      cout<<"slices start:\n";
      printVector(slices_start);
      cout<<"tuples:\n";
      printVector(tuples);
    }

    /*
     * It takes an output file stream object and prints the solution in the given file object.
     */
    void print_solution(ofstream& fout){
      fout<<"paths: ";
      printVector(paths, fout);
      fout<<"slices occupied:\n";
      printVector(slices_req, fout);
      fout<<"slices start:\n";
      printVector(slices_start, fout);
      fout<<"tuples:\n";
      printVector(tuples, fout);
    }

};


#endif //HUA_TO_MODIFY_SOLUTION_H
