//
// Created by sepehr on 07/09/18.
//

#ifndef VNE_PROTECTION_CONVERTOR_H
#define VNE_PROTECTION_CONVERTOR_H

#include <iostream>
#include <vector>
#include <map>
using namespace std;

/*
 * this class contains deals with the finding all multisets of the shortest path between two snodes which number of members is less than
 * maximum number of splits.
 * It keeps an order for all the multisets of k shortest paths between any pair of snodes.
 * each combination is in the form of:
 * [<number-of-usage-1st path>, <number-of-usage-2nd path>, ..., <number-of-usage-kth path>, max-number-of-splits - sum-of-other-members]
 * Therefore, sum of members is always equal to max number of splits
 */
class convertor {
private:
    int sum; // max number of splits
    int n; // number of shortest paths
    int number_of_combinations; // keeps number of these combinations

    /*
     * each combination is a list of size n+1 which sum of its elements is equal to the sum.
     * The combination value is calculated by considering the combination in base n+1
     */
    map<int, int> combination_to_index; // a map from a combination value to its index
    map<int, int> index_to_combination; // a map from a index to a combination value


    // returns the next combination, which is the combination with smallest value greater than current combination value
    vector<int> next_combination(vector<int> combination);

    // calculate the combination value
    int combination_to_int(vector<int> combination);

    // generate all the combinations in order
    void initializer();

public:

    // the constructor of the class
    convertor(int number_of_paths, int number_of_splits);

    // returns number of combinations
    int get_number_of_combinations();

    // returns the index of a combination
    int get_combination_index(vector<int> combination);

    // returns the combination given its value
    vector<int> int_to_combination(int value);

    /*
     * decompose a combination (given its value) to set of combinations each of which consists of only one of
     * the paths.
     * for example: n = 4 q = 4 and combination: (2, 0, 1, 1, 0) decomposed into:
     *   (2, 0, 0, 0, 2), (0, 0, 1, 0, 3) and (0, 0, 0, 1, 3).
     */
    vector<vector<int>> get_parent_cases(int combination_value);

    // a debugging function
    void print_all_combinations_in_order();

    /*
     * a base combination is a combination which use only one of the shortest paths.
     * e.g., (4, 0, 0, 0, 0) is a base combination --> it only use first path
     * but (0, 2, 1, 0, 1) is not a base combination since it has both 2nd and 3rd paths
     */
    bool is_base_combination(vector<int> combination);

    /*
     * returns the number of distinct paths that are part of a combination
     * e.g., (2, 1, 1, 0, 0) --> 3 (paths 1, 2, 3)
     */
    int get_number_of_distinct_members(vector<int> combination);

    // returns the value of a combination given its index
    int get_combination_value_by_index(int combination_index);

    /*
     * returns a multiset which the multiplicity of a path id is equal to the number of usage of that path in the combination
     * e.g., (1, 2, 0, 1, 0) --> {1, 2, 2, 4} (our function is zero based so result would be {0, 1, 1, 3})
     */
    vector<int> get_combination_path_ids(vector<int> combination);

};


#endif //VNE_PROTECTION_CONVERTOR_H
