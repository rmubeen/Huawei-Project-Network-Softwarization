#include <map>
#include <queue>
#include <bitset>
#include <iostream>
#include <algorithm>    // std::find
#include <vector>       // std::vector
#include <chrono>
#include <cmath>
#include <fstream>
#include <assert.h>
#include "problem_spec.h"
#include "reading_utilities.h"
#include "split_option_finder.h"
#include "solution.h"
#include "reach_table.h"
#include "convertor.h"
#include "io.h"
#include "../basic_functions.cpp"
//#include "basic_functions.cpp"


#define MAX 1.0e10
#define CONNECTIVITY 3
#define MAX_instance 4
#define threshold 12
#define factor 0.01


//////////////sepehr

using namespace std;

const std::string kUsage = "./vne_protection "
                           "--pn_topology_file=<pn_topology_file>\n\t"
                           "--vn_topology_file=<vn_topology_file>\n\t"
                           "--location_constraint_file=<location_constraint_file>";



int main(int argc, char *argv[]) {
    auto arg_map = ParseArgs(argc, argv);
    string pn_topology_filename = "";
    string vn_topology_filename = "";
    string location_constraint_filename = "";
    for (auto argument : *arg_map) {
        if (argument.first == "--pn_topology_file") {
            pn_topology_filename = argument.second;
        } else if (argument.first == "--vn_topology_file") {
            vn_topology_filename = argument.second;
        } else if (argument.first == "--location_constraint_file") {
            location_constraint_filename = argument.second;
        } else {
            printf("Invalid command line option: %s\n", argument.first.c_str());
            return 1;
        }
    }
    string paths_file = vn_topology_filename + ".path";

    cout<<paths_file<<endl;
    cout<<vn_topology_filename<<endl;
    cout<<location_constraint_filename<<endl;


    int q = 4;
    int k = 10; //k shortest path
//    string paths_file = "/home/sepehr/University/Huawei Project/topologies/varl2nvn_with_path/case0/vnr/vn0.txt.path";
//    string vn_file = "/home/sepehr/University/Huawei Project/topologies/varl2nvn_with_path/case0/vnr/vn0.txt";
//    string vn_loc_file = "/home/sepehr/University/Huawei Project/topologies/varl2nvn_with_path/case0/vnr/vnloc0.txt";

//  INITIALIZING DATA
    problem_spec vn(paths_file, vn_topology_filename, location_constraint_filename);
    convertor convertor_instance(k, q);
    string table_path = "ONE_v1.csv";
//    cout << "CP1\n";
    reach_table reach_table_instance(table_path, q, k);
//    cout << "CP2\n";

/*
    cout << "Printing problem_spec VN\n";

    cout << "vLinks\n";
    print2DVector(vn.vlinks);
    cout << endl;

    cout << "vn Shortest Paths\n";
    print2DVector(vn.shortest_paths);
    cout << endl;

    cout << "vn Location Constraint\n";
    print2DVector(vn.loc_constraint);
    cout << endl;

    cout << "vLink Shortest Path Indexes\n";
    print2DVector(vn.vlink_shortest_paths_indexes);
    cout << endl;

    cout << "vn Adjancy List\n";
    print2DVector(vn.adjacency_list);
    cout << endl;

    cout << "vLink Order\n";
    printVector(vn.vlink_order);
    cout << endl;

    cout << "vn Path Degree to Next vLinks\n";
    print2DVector(vn.path_degree_to_next_vlinks);
    cout << endl;
*/
/*
    cout << "Printing convertor convertor_instance\n";
    cout << "number of combinations\n";
    cout << convertor_instance.get_number_of_combinations() << endl;

    cout << "int to combinations\n";
    printVector(convertor_instance.int_to_combination(300));
    cout << endl;

    cout << "index of combinations\n";
    cout << convertor_instance.get_combination_index(convertor_instance.int_to_combination(300));
    cout << endl;
*/
/*
    cout << "Printing reach_table reach_table_instance\n";
    cout << "get all bit rates vector\n";
    printVector(reach_table_instance.get_all_bit_rates_vector());
    cout << endl;

    cout << "get all sum combinations\n";
    print2DVector(**reach_table_instance.get_all_sum_combinations());

    cout << "get all permutations\n";
    print2DVector(reach_table_instance.get_all_permutations(300));

    cout << "get all bit rate combinations\n";
    print2DVector(reach_table_instance.get_all_bit_rate_combination_with_specific_sum(8, 4));

    cout << "get number of bit rates\n";
    cout << reach_table_instance.get_number_of_bit_rates() << endl;

    cout << "get bit rate index\n";
    cout << reach_table_instance.get_bit_rate_index(300) << endl;

    cout << "get best tuple\n";
    printVector(reach_table_instance.get_best_tuple(400, 300));
*/
    double cost = 0;
    vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> path_slices(vn.shortest_paths.size(), bitset<NUMBER_OF_PHYSICAL_SLICES> (0));

    for(int i = 0; i < vn.vlink_order.size(); i++) {
        // FOR EACH VLink
        int vlink_index = vn.vlink_order[i];
        int bit_rate = vn.vlinks[vlink_index][3];

        vector<bitset<NUMBER_OF_PHYSICAL_SLICES> > current_vlink_path_slices;
        vector<int> distances;
        vector<int> number_of_hops;
        vector<vector<int>> adj_list;
        vector<vector<int>> path_degrees;
        vector<int> next_demands;

        for(int j = i+1; j < vn.vlink_order.size(); j++) {
            int index = vn.vlink_order[j];
            int demand = vn.vlinks[index][3];
            next_demands.push_back(demand);
        }

        int starting_path_index = vn.vlink_shortest_paths_indexes[vlink_index][2];
        int ending_path_index = vn.vlink_shortest_paths_indexes[vlink_index][3];

//        cout << "priting next demands vec\n";
//        printVector(next_demands);
//        cout << endl;

        for(int j = starting_path_index; j < ending_path_index; j++) {
            // FOR EACH PATH OF VLink
            current_vlink_path_slices.push_back(path_slices[j]);

            number_of_hops.push_back(vn.shortest_paths[j][6]);
            distances.push_back(vn.shortest_paths[j][7]);
            path_degrees.push_back(vn.path_degree_to_next_vlinks[j]);

            vector<int>::iterator lb_adj_index = lower_bound(vn.adjacency_list[j].begin(), vn.adjacency_list[j].end(), starting_path_index);
            vector<int>::iterator ub_adj_index = upper_bound(vn.adjacency_list[j].begin(), vn.adjacency_list[j].end(), ending_path_index);

            vector<int> current_path_adj_list(lb_adj_index, ub_adj_index);

            adj_list.push_back(vector<int>(0));
            for(int k = 0; k < current_path_adj_list.size(); k++)
                adj_list[adj_list.size()-1].push_back(current_path_adj_list[k]-starting_path_index);
        }


        print2DVector(adj_list);
    		printPy("\n");

        split_option_finder solver(bit_rate, q, current_vlink_path_slices, distances, number_of_hops, adj_list,  &convertor_instance, &reach_table_instance, path_degrees, next_demands);
//        split_option_finder solver
//              (bit_rate, q, current_vlink_path_slices, distances,
//              number_of_hops, adj_list,  &convertor_instance,
//              &reach_table_instance, path_degrees, next_demands);


        vector<solution> solutions = solver.get_solutions_vector();
//        cout<<"solution size: "<<solutions.size()<<endl;
        if(solutions.size() == 0) {
            cout << "NO Solution ----> " << i<<"/"<<vn.vlink_order.size()<< endl;
            return 0;
        }
        cost += solutions[0].get_needed_slices();
        cost += solutions[0].get_paths().size() * 0.1;

        vector<int> paths = solutions[0].get_paths();
        vector<int> starting_slices = solutions[0].get_starting_slices();
        vector<int> tuples_indexes = solutions[0].get_tuples();
/*
        if (true) {
          printVector(paths); cout << endl;
          printVector(starting_slices); cout << endl;
          printVector(tuples_indexes); cout << endl; cout << endl;
        }
*/
//        solver.print_solutions();

        for(int j = 0; j < paths.size(); j++) {
            int path_index = paths[j] + starting_path_index;
            int needed_slice = reach_table_instance.get_tuple_needed_slices(tuples_indexes[j]);

            for(int k = 0; k < needed_slice; k++)
                path_slices[path_index].set(starting_slices[j] + k);

            for(int k = 0; k < vn.adjacency_list[path_index].size(); k++) {
                int adj_index = vn.adjacency_list[path_index][k];

                for(int x = 0; x < needed_slice; x++)
                    path_slices[adj_index].set(starting_slices[j] + x);
            }
        }
//        solver.print_solutions();
//        for(int j = 0; j < path_slices.size(); j++)
//            cout<<path_slices[j].to_string<char, string::traits_type, string::allocator_type>()<<endl;
solver.print();
    }
    cout<<cost<<endl;

    /*
    convertor convertor_instance(k, q);
    string table_path = "/home/sepehr/CLionProjects/hua-huristic/ONE_v1.csv";
    reach_table reach_table_instance(table_path, q, k);

    vector<bitset<NUMBER_OF_PHYSICAL_SLICES> > path_slices;
    path_slices.push_back(bitset<NUMBER_OF_PHYSICAL_SLICES> (string("000111111111111111111111111110001111111100010000")));
    path_slices.push_back(bitset<NUMBER_OF_PHYSICAL_SLICES> (string("111111111111111111111111000001111111111111111111")));
    path_slices.push_back(bitset<NUMBER_OF_PHYSICAL_SLICES> (string("000000111110000001110000000001111100000001111111")));
    path_slices.push_back(bitset<NUMBER_OF_PHYSICAL_SLICES> (string("000000111110000010101010000000000000000001111111")));
    path_slices.push_back(bitset<NUMBER_OF_PHYSICAL_SLICES> (string("111000110110000000011010000111111110000001111111")));

    path_slices.push_back(bitset<NUMBER_OF_PHYSICAL_SLICES> (string("000111111111111111111111111110001111111100010000")));
    path_slices.push_back(bitset<NUMBER_OF_PHYSICAL_SLICES> (string("111111111111111111111111000001111111111111111111")));
    path_slices.push_back(bitset<NUMBER_OF_PHYSICAL_SLICES> (string("000000111110000001110000000001111100000001111111")));
    path_slices.push_back(bitset<NUMBER_OF_PHYSICAL_SLICES> (string("000000111110000010101010000000000000000001111111")));
    path_slices.push_back(bitset<NUMBER_OF_PHYSICAL_SLICES> (string("111000110110000000011010000111111110000001111111")));

    int bit_rate = 1000;
    vector<int> distances({1000, 410, 250, 742, 200});
    vector<int> number_of_hops({5, 2, 2, 2, 4});

    vector<vector<int>> adj_list;
    adj_list.push_back(vector<int>({1, 2, 5}));
    adj_list.push_back(vector<int>({0, 3, 7, 8}));
    adj_list.push_back(vector<int>({0, 3, 9}));
    adj_list.push_back(vector<int>({1, 2, 6, 5}));
    adj_list.push_back(vector<int>(0));

    split_option_finder solver(bit_rate, q, path_slices, distances, number_of_hops, adj_list,  &convertor_instance, &reach_table_instance);


    vector<solution> solutions = solver.get_solutions_vector();

    solver.print_solutions();
    */
    return 0;
}
