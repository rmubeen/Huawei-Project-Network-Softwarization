//
// Created by sepehr on 07/09/18.
//

#ifndef VNE_PROTECTION_SPLIT_OPTION_FINDER_H
#define VNE_PROTECTION_SPLIT_OPTION_FINDER_H


#include <iostream>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>
#include <math.h>
#include "convertor.h"
#include "reach_table.h"
#include "solution.h"
#include "hua_disjoint_paths.cpp"
#include "basic_functions.cpp"
using namespace std;

#define MAX_CH 12
//#define MAX_CH 48

const int NUMBER_OF_PHYSICAL_SLICES = MAX_CH;

/*
 * this class finds the all the solution based on the current state of the substrate network for a single vlink (given
 * a vnode mapping)
 */
class split_option_finder {
private:
    int bit_rate; // demand of the vlink
    int number_of_paths; // number of shortest path between src and dst
    int max_number_of_splits; // maximum number of splits
    convertor *convertor_instance; // an instance of the convertor class
    reach_table *reach_table_instance; // an instance of the reach_table class
    vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> path_slices; // the current state of the spectrum occupation of a shortest path
    vector<vector<int>> adj_list; // adjacency list for all the shortest paths. Two path are adjacent iff they have at least one common link.
    vector<vector<int>> path_degrees;
    vector<int> next_demands;
    protectedPathsOfPair_S allVNPaths;
    int min_slices;
    vector<vector<solution>> vnSolutions;
    solution optimalSolution;
    bool debug;
    double bsr_value;

    /*
     * find the first place (first-fit) that there exists needed_slices number of contiguous unused slices in this path.
     * returns -1 if there is no solution.
     */
    int enough_space(int needed_slices, bitset<NUMBER_OF_PHYSICAL_SLICES> p);

    void solveProtectedPaths(vector<vector<int>> paths, int index_of_path, int* numOfSlices);
    vector<pair<int, int>> divide_and_merge(vector<int> division, vector<vector<int>> paths);
    vector<pair<int, vector<vector<int>>> > calculateFinalBWDiv(vector<pair<int, int>> paths_bw, vector<int> split_comb, vector<int> bit_rates);
    solution solution_feasible(vector<pair<int, vector<int>>> solution);
    bool is_available(int start, int num, bitset<NUMBER_OF_PHYSICAL_SLICES> pathSlices);
    int firstFitAllocationConsideringAdjacentPaths(int pathId, int slicesReq, vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> pathSlices, vector<int> adjPaths);
    solution return_solution(vector<pair<int, vector<int>>> bitrateDiv);
    vector<int> find_next_comb(vector<int> cur, vector<int> max);

public:
    /*
     * It gets the demand of the vlink (bit_rate), max number of splits (q), state of the spectrum slices usage of
     * all the paths that could be used for the current or future vlinks (based on sorting algorithm that we used
     * at first over set of vlinks) (slices), distance of the paths related to the current vlink (distances), number
     * of hops along each of the current vlink paths (hops), adjacency list for the all the paths (current vlink and
     * next vlinks) (adjacency_list), an instance of convertor (convertor_ins) and an instance of reach_table
     * (reach_table_ins)
     *
     * And call the solve_dp function to solve the problem for this vlink.
     */
      split_option_finder(bool DBG, double bsr_value, int Q, int bit_rate, vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> slices, protectedPathsOfPair_S paths, reach_table *reach_table_ins, vector<vector<int>> path_degrees, vector<int> next_demands, vector<vector<int>> adj_list);

      vector<vector<solution>> get_solutions(){
          return this->vnSolutions;
      }

      solution get_optimal_solution(){
          return this->optimalSolution;
      }
};


#endif //VNE_PROTECTION_SPLIT_OPTION_FINDER_H
