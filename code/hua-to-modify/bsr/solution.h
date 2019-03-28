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

class solution {
private:
    vector<int> paths;
    vector<int> slices_req;
    vector<int> slices_start;
    vector<int> tuples;
    int needed_slices;

    vector<int> next_demands;
    vector<vector<int>> path_degrees;
/*
    double cost;
    reach_table *reach_table_instance;

    void calculate_cost() {
        cost = 0;
        for(int i = 0; i < paths.size(); i++) {
            int path_id = paths[i];
            int needed_s = path_needs[i];
            for(int j = 0; j < path_degrees[path_id].size(); j++)
                cost +=  needed_s * (next_demands[j]/100) * path_degrees[path_id][j];
        }
    }*/

public:
    solution(vector<int> paths, vector<int> tuples, vector<int> slices_req, vector<int> starting_slices_index, int needed_slices, vector<vector<int>> path_degrees, vector<int> next_demands) {
        this->paths = paths;
        this->tuples = tuples;
        this->slices_start = starting_slices_index;
        this->slices_req = slices_req;
        this->needed_slices = needed_slices;
        this->path_degrees = path_degrees;
        this->next_demands = next_demands;

//        calculate_cost();
    }

//    solution(vector<int> paths, vector<int> tuples, vector<int> starting_slices_index, int needed_slices, vector<vector<int>> path_degrees, reach_table *reach_table_ins, vector<int> next_demands) {


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
        return slices_start;
    }

    vector<int> get_req_slices() {
        return slices_req;
    }

/*    double get_cost() {
        return cost;
    }*/

    int get_needed_slices() {
        return needed_slices;
    }


//    const bool operator < (const solution &other_solution) const {
//        if(this->needed_slices != other_solution.needed_slices)
//            return this->needed_slices < other_solution.needed_slices;
//        return this->paths.size() < other_solution.paths.size();
//    }

/*    const bool operator < (const solution &other_solution) const {
        if(this->cost != other_solution.cost)
            return this->cost < other_solution.cost;
        if(this->needed_slices != other_solution.needed_slices)
            return this->needed_slices < other_solution.needed_slices;


        return this->paths.size() < other_solution.paths.size();
    }*/

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

    void print_solution(ofstream& fout){
      fout<<"needed_slices: " << needed_slices << endl;
      fout<<"paths: ";
      printVector(paths, fout);
      fout<<"slices required:\n";
      printVector(slices_req, fout);
      fout<<"slices start:\n";
      printVector(slices_start, fout);
      fout<<"tuples:\n";
      printVector(tuples, fout);
    }

};


#endif //HUA_TO_MODIFY_SOLUTION_H
