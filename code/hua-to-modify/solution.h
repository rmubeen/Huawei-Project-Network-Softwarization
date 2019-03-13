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
    vector<vector<int>> path_degrees;
    vector<int> tuples; // tuples ids
    vector<int> starting_slices;
    int needed_slices;
    double cost;
    reach_table *reach_table_instance;
    vector<int> next_demands;

    void calculate_cost() {
        cost = 0;
        for(int i = 0; i < paths.size(); i++) {
            int path_id = paths[i];
            int needed_s = reach_table_instance->get_tuple_needed_slices(tuples[i]);
            for(int j = 0; j < path_degrees[path_id].size(); j++)
                cost +=  needed_s * (next_demands[j]/100) * path_degrees[path_id][j];
        }
    }

public:
    solution(vector<int> paths, vector<int> tuples, vector<int> starting_slices_index, int needed_slices, vector<vector<int>> path_degrees, reach_table *reach_table_ins, vector<int> next_demands) {
        this->paths = paths;
        this->tuples = tuples;
        this->starting_slices = starting_slices_index;
        this->needed_slices = needed_slices;
        this->path_degrees = path_degrees;
        reach_table_instance = reach_table_ins;
        this->next_demands = next_demands;
        calculate_cost();
    }

    solution() { //create an invalid solution
        this->needed_slices = -1;
    }

    vector<int> get_paths() {
        return paths;
    }

    vector<int> get_tuples() {
        return tuples;
    }

    vector<int> get_starting_slices() {
        return starting_slices;
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
