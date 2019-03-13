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
#include "convertor.h"
#include "reach_table.h"
#include "solution.h"
using namespace std;

#define MAX_CH 12

const int NUMBER_OF_PHYSICAL_SLICES = MAX_CH;

/*
 * this class finds the all the solution based on the current state of the substrate network for a single vlink (given
 * a vnode mapping)
 */
class split_option_finder {
private:
    int bit_rate; // demand of the vlink
    int src; // the snode that source of the vlink is mapped to
    int dst; // the snode that destination of the vlink is mapped to
    int number_of_paths; // number of shortest path between src and dst
    int max_number_of_splits; // maximum number of splits
    convertor *convertor_instance; // an instance of the convertor class
    reach_table *reach_table_instance; // an instance of the reach_table class
    vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> path_slices; // the current state of the spectrum occupation of a shortest path
    vector<int> number_of_hops; // number of hops along a shortest path
    vector<int> path_distances; // distance of a shortest path
    vector<vector<int>> adj_list; // adjacency list for all the shortest paths. Two path are adjacent iff they have at least one common link.
    vector<vector<int>> path_degrees;
    vector<int> next_demands;

    /*
     * dynamic_table[bit_rate_index][combination_index]. It keeps the best solution (least number of needed slices)
     * for each bit rate and each combination of shortest paths.
     */
    solution **dynamic_table;

    vector<solution> solution_array; //all the possible solutions that satisfy the demand of the vlink

    /*
     * find the first place (first-fit) that there exists needed_slices number of contiguous unused slices in this path.
     * returns -1 if there is no solution.
     */
    int enough_space(int needed_slices, bitset<NUMBER_OF_PHYSICAL_SLICES> p);

    /*
     * returns the best solution (least number of needed slices) for a base case.
     * A base case is a case that only use one of the shortest path (maybe several times)
     * to satisfy a vlink demand.
     * It goes through all the permutations over the tuples and tried to fit each of them
     * in the path using enough_space function (first fit).
     */
    solution number_of_slices_needed_base_cases(int path_id, vector<vector<int>> best_tuples, int current_solution_obj);

    /*
     * This function find and return the best solution for the cases other than base cases.
     * (cases that use more than one path)
     * It goes through all the permutation of the tuples and tried to fit them in their relating
     * paths using the first-fit algorithm.
     */
    solution find_solution(vector<int> path_ids, vector<int> tuple_ids);

    /*
     * That's the main function to solve the problem for a single vlink using dynamic programming approach.
     * It first solve the problem for all the pairs of base cases and a bit_rates (less than the demands).
     * A base case is a multiset of size at most q (max number of splits) from shortest paths between src and
     * dst.
     * For each pair of <base_case, bit_rate> it goes through all the possible ordered sets that each member of
     * them is a bit_rate in all_bit_rates and the sum of them is equal to bit_rate, and find the best combination
     * (least number of spectrum slices usage).
     *
     * Then using the base cases it solves all other cases. For other cases it first decompose that into base cases
     * and use the solution that it has already founded and solve the problem for those cases.
     *
     * At the end it goes through all the combination that has demand equal to the demand of the vlink, if there is
     * a solution then it adds that solution to the set of solution.
     * Finally, sorts all the solutions by the number of needed_slices.
     */
    void solve_dp();

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
    split_option_finder(int bit_rate, int q, vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> slices, vector<int> distances, vector<int> hops, vector<vector<int>> adjacency_list, convertor *convertor_ins, reach_table *reach_table_ins, vector<vector<int>> path_degrees, vector<int> next_demands);

    // returns a vector of solution that has been sorted based on number of needed slices
    vector<solution> get_solutions_vector();

    // a debugging function
    void print();

    // a debugging function
    void print_solutions();

};


#endif //VNE_PROTECTION_SPLIT_OPTION_FINDER_H
