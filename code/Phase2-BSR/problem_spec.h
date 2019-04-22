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
    vector<vector<int>> vlink_shortest_paths_indexes; // index range of shortest paths for each vLink.
    vector<vector<int>> adjacency_list; // It has a vector of paths, for each path, which share some common link with the path.
    vector<int> vlink_order;
    vector<vector<int>> path_degree_to_next_vlinks;

    /*
     * It takes VN topology files as input, reads them, and stores VN topology.
     */
    problem_spec(string path_file_path, string vn_path, string vn_loc_path);

    /*
     * It reads file with the given filename.
     */
    void read_file(file_type input_file_type);

    /*
     * It creates a vector of adjacent paths (paths which share a physical link) for each path..
     */
    void create_adjacency_list();
    void find_best_order();
    void find_path_degrees_to_next_vlinks();
};


#endif //HUA_TO_MODIFY_GET_VN_SCPECIFICATION_H
