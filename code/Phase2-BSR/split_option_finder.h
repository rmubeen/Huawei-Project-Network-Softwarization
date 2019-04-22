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

  /*
   * Short Fix CONF
   */
#define MAX_CH 12

  /*
   * Short Flex CONF
   */
//#define MAX_CH 48

  /*
   * Long Fix CONF
   */
//#define MAX_CH 80

  /*
   * Long Flex CONF
   */
//#define MAX_CH 320

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
    vector<int> next_demands; // demands for next vLinks for look ahead
    protectedPathsOfPair_S allVNPaths; // all protected paths for the vLink
    int min_slices; // minimu needed slices for the best solution
    vector<vector<solution>> vnSolutions; // solutions vector
    solution optimalSolution; // solution with minimum needed slices
    bool debug; // debug flag
    double bsr_value; // bsr rate i.e. 20% 40% 60% etc.
    vector<int> all_bit_rates; // all bit rates in the reach tables

    vector<vector<vector<vector< vector<int> >>>> dynamicTable; // dynamicTable[path_index][bit_index][split][division_index] for DP

    /*
     * find the first place (first-fit) that there exists needed_slices number of contiguous unused slices in this path.
     * returns -1 if there is no solution.
     */
    int enough_space(int needed_slices, bitset<NUMBER_OF_PHYSICAL_SLICES> p);

    /*
     * It solves given protected path.
     */
    void solveProtectedPaths(vector<vector<int>> paths, int index_of_path);

    /*
     * It divides bit-rate of each disjoint set in a protected path and calculates final bit-rate division of each unique path.
     */
    vector<pair<int, int>> divide_and_merge(vector<int> division, vector<vector<int>> paths);

    /*
     * It divides bit-rate of each unique path in a protected path to the given number of splits as per split combination.
     */
    vector<pair<int, vector<vector<int>>> > calculateFinalBWDiv(vector<pair<int, int>> paths_bw, vector<int> split_comb, vector<int> bit_rates);

    /*
     * It takes a possible solution of given vLink bit-rate according to reach table.
     * And, checks if it is possible to embed that solution on required vLink paths given their current occupancy.
     */
    solution solution_feasible(vector<pair<int, int>> solutionVec, int needed_slices);

    /*
     * It takes starting index, number of slices required and current slice allocatoion.
     * Then, returns true if the required number of slices are available on the given path starting from given index.
     * And, returns false otherwise.
     */
    bool is_available(int start, int num, bitset<NUMBER_OF_PHYSICAL_SLICES> pathSlices);

    /*
     * It takes a bit-rate division and current best solution.
     * Then, returns a valid solution if the new bit-rate division can produce a better solution than the current best solution.
     * And, returns an invalid solution otherwise.
     */
    solution return_solution(vector<pair<int, vector<int>>> bitrateDiv, int cur_min);

    /*
     * It takes a vector combination of bit-rate divisions as per split combinations and the upper bound.
     * Then, returns the next combination.
     */
    vector<int> find_next_comb(vector<int> cur, vector<int> max);

    /*
     * It fills the dynamic table for different combinations before computing the solution to minimize the time for calculating best solution.
     */
    void fill_dynamic_table(int K);

    /*
     * It takes bit rate division on virtual paths in a protected path then returns reach table tuple ids if that bit rate division is possible.
     * As per dynamic table, and returns invalid vector of tuple ids otherwise.
     */
    vector< pair<int, vector<vector<int>>> > check_final_bw_div_in_table(vector< pair<int, vector<vector<int>>> > final_bw_div, vector<pair<int, int>> paths_bw);

public:
    /*
     * It gets the demand of the vlink (bit_rate), max number of splits (q), state of the spectrum slices usage of
     * all the paths that could be used for the current or future vlinks (based on sorting algorithm that we used
     * at first over set of vlinks) (slices), distance of the paths related to the current vlink (distances), number
     * of hops along each of the current vlink paths (hops), adjacency list for the all the paths (current vlink and
     * next vlinks) (adjacency_list), protected paths of vLink, an instance of convertor (convertor_ins) and an instance of reach_table
     * (reach_table_ins)
     *
     * And computes solutions(embeddings) for given bit-rate.
     */
      split_option_finder(bool DBG, int K, double bsr_value, int Q, int bit_rate, vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> slices, protectedPathsOfPair_S paths, reach_table *reach_table_ins, vector<vector<int>> path_degrees, vector<int> next_demands, vector<vector<int>> adj_list);


     /*
      * It returns all solutions for the given requirements.
      */
      vector<vector<solution>> get_solutions(){
          return this->vnSolutions;
      }

      /*
       * It returns best solution (with minimum needed slices) for the given requirements.
       */
      solution get_optimal_solution(){
          return this->optimalSolution;
      }
};


#endif //VNE_PROTECTION_SPLIT_OPTION_FINDER_H
