//
// Created by sepehr on 10/08/18.
//

#ifndef HUA_TO_MODIFY_GET_VN_SCPECIFICATION_H
#define HUA_TO_MODIFY_GET_VN_SCPECIFICATION_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "reading_utilities.h"
using namespace std;


class problem_spec {
public:
    enum file_type {path, vn, vn_loc};
    ifstream fin;
    vector<vector<int>> vlinks; // 0: id, 1: src, 2: dst, 3: demand
    vector<vector<int>> shortest_paths; // 0: id 1: v_src, 2: v_dst, 3: s_src, 4: s_dst, 5: index (ordered), 6: hops, 7: distance, {hops}
    vector<vector<int>> loc_constraint; // 0: v_node, snodes
    vector<vector<int>> vlink_shortest_paths_indexes;
    vector<vector<int>> adjacency_list;
    vector<int> vlink_order;
    vector<vector<int>> path_degree_to_next_vlinks;

    problem_spec(string path_file_path, string vn_path, string vn_loc_path);
    void read_file(file_type input_file_type);
    void create_adjacency_list();
    void find_best_order();
    void find_path_degrees_to_next_vlinks();
};


#endif //HUA_TO_MODIFY_GET_VN_SCPECIFICATION_H
