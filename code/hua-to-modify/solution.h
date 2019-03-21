//
// Created by sepehr on 08/09/18.
//

#ifndef HUA_TO_MODIFY_SOLUTION_H
#define HUA_TO_MODIFY_SOLUTION_H

#include <vector>
#include "reach_table.h"
using namespace std;

class solution {
private:
    vector<int> paths;
    vector<vector<int>> slices_req;
    vector<vector<int>> slices_start;
    vector<vector<int>> tuples;
    vector<int> path_needs;

    vector<vector<int>> path_degrees;
//    vector<int> tuples; // tuples ids
    vector<int> starting_slices;
    int needed_slices;
    double cost;
    reach_table *reach_table_instance;
    vector<int> next_demands;

    void calculate_cost() {
        cost = 0;
        for(int i = 0; i < paths.size(); i++) {
            int path_id = paths[i];
            int needed_s = path_needs[i];
            for(int j = 0; j < path_degrees[path_id].size(); j++)
                cost +=  needed_s * (next_demands[j]/100) * path_degrees[path_id][j];
        }
    }

public:
    solution(vector<int> paths, vector<vector<int>> tuples, vector<vector<int>> starting_slices_index, vector<vector<int>> slices_req, vector<vector<int>> path_degrees, vector<int> next_demands) {
        this->paths = paths;
        this->tuples = tuples;
        this->slices_start = starting_slices_index;
        this->slices_req = slices_req;
        this->path_needs = {};
        this->needed_slices = 0;
        this->path_degrees = path_degrees;
        this->next_demands = next_demands;

        for (int i = 0; i < slices_req.size(); i++) {
            int subTotal = 0;
            for (int j = 0; j < slices_req[i].size(); j++) {
                subTotal += slices_req[i][j];
            }
            this->path_needs.push_back(subTotal);
            this->needed_slices += subTotal;
        }

//        reach_table_instance = reach_table_ins;
        calculate_cost();
    }

//    solution(vector<int> paths, vector<int> tuples, vector<int> starting_slices_index, int needed_slices, vector<vector<int>> path_degrees, reach_table *reach_table_ins, vector<int> next_demands) {


    solution() { //create an invalid solution
        this->needed_slices = -1;
    }

    vector<int> get_paths() {
        return paths;
    }

    vector<vector<int>> get_tuples() {
        return tuples;
    }

    vector<vector<int>> get_starting_slices() {
        return slices_start;
    }

    vector<vector<int>> get_req_slices() {
        return slices_req;
    }

    double get_cost() {
        return cost;
    }

    int get_needed_slices() {
        return needed_slices;
    }


//    const bool operator < (const solution &other_solution) const {
//        if(this->needed_slices != other_solution.needed_slices)
//            return this->needed_slices < other_solution.needed_slices;
//        return this->paths.size() < other_solution.paths.size();
//    }

    const bool operator < (const solution &other_solution) const {
        if(this->cost != other_solution.cost)
            return this->cost < other_solution.cost;
        if(this->needed_slices != other_solution.needed_slices)
            return this->needed_slices < other_solution.needed_slices;


        return this->paths.size() < other_solution.paths.size();
    }
};


#endif //HUA_TO_MODIFY_SOLUTION_H
