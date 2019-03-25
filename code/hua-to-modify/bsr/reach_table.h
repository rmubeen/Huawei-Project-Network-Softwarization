//
// Created by sepehr on 07/09/18.
//

#ifndef VNE_PROTECTION_REACH_TABLE_H
#define VNE_PROTECTION_REACH_TABLE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <map>
#include "reading_utilities.cpp"

using namespace std;

//#define slice_width 50
#define slice_width 12.5


// reach_table class deals with the reading and storing the reach_table (tuples).
class reach_table{
private:
    double slice_size = slice_width; // width of each slice

    vector<vector<int>> tuples; // keeps all the tuples in the reach table. 0: id, 1: bit_rate, 2: distance, 3: needed_slices (based on width)
    map<int, int> tuple_bit_rate_to_first_index; // maps each bit rate to the first index of that bit rate (tuples sorted by their bit rates)
    vector<int> all_bit_rates; // all bit rates that come in the reach table (including intermediate bit rates)
    map<int, int> bit_rate_index; // index of a bit rate in all_bit_rates vector
    int max_number_of_splits; // max number of splits for a vlink (q in the formulation)
    int number_of_paths; // number of shortest path between each pair of snodes

    vector<vector<int>> **base_combinations; // keeps all the multiset from tuple_bit_rates that sum of its members is equal to a certain value. base_combinations[index of the sum in all_bit_rates][size of multiset]= {{   }, {   }, {    }}
    vector<vector<int>> **sum_combinations; // keeps all the ordered multiset (all permutation of a multiset) from all bit_rates that sum of its members is equalt to a certain value. sum_combination[index of the sum in all_bit_rates][number_of_splits]
    vector<vector<int>> *permutations; // permutation[i]: all the permutations of (0, 1, .., i-1)

    // a debugging fuction
    void check();

    // a debugging function

    /*
     * generate all the permutations of the multisets from all_bit_rates with a specific size and sum of members.
     * It generates these permutations for all sum in all_bit_rates. And size of the multisets could be any value
     * between 1 and number of shortest paths
     */
    void generate_all_sum_combinations();

    /*
     * generate all the multisets from tuple bit rates with a specific size and sum of members.
     * It generates these multisets for all sum in all_bit_rates and all size from 1 and max number of splits.
     */
    void generate_all_base_combinations();

    // a debugging function
    void print_all_permutations();

    /*
     * generate all permutations of the members of the set {0, 1, .., i-1} for all i between 1 and max number of splits
     */
    void generate_all_permutations();

public:
  void print_sum_combinations();
  void print_base_combinations();
    /*
     * the constructor of this class that gets the path of the reach_table file and some property of the problem.
     * Read the reach table and store that in relating variables of the object.
     */
    reach_table(string file_path, int max_number_of_splits, int number_of_paths, int max_possible_data_rate = 1000, int smallest_bit_rate = 100, int bit_rate_step = 50);

    // returns all the bit rates (including intermediate bit_rates, bit rates other than bit rates in tuples)
    vector<int> get_all_bit_rates_vector();

    // returns number of needed slices for a specific tuple
    int get_tuple_needed_slices(int reach_table_index);

    // returns the whole all_sum_combination variable
    vector<vector<int>>** get_all_sum_combinations();

    // returns all permutations of the members of set {0, 1, ..., n-1}
    vector<vector<int>> get_all_permutations(int n);

    // returns all the multisets with specific size and specific sum for the members
    vector<vector<int>> get_all_bit_rate_combination_with_specific_sum(int sum_index, int number_of_splits);

    // returns number of the bit rates (including intermediate bit rates)
    int get_number_of_bit_rates();

    // returns the index of a bit rate in the all_bit_rates vector
    int get_bit_rate_index(int bit_rate);

    // returns the best tuple (least needed slice) for a given distance and data_rate
    vector<int> get_best_tuple(int distance, int data_rate);

};


#endif //VNE_PROTECTION_REACH_TABLE_H
