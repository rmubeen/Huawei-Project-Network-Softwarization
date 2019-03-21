#include <iostream>
#include <fstream>
#include <bitset>
#include "io.h"
#include "basic_functions.cpp"
#include "problem_spec.cpp"
#include "split_option_finder.cpp"
#include "solution.h"
#include "convertor.cpp"
#include "reach_table.cpp"
#include "bsr/hua_disjoint_paths.cpp"
using namespace std;

#define Q 4 //max number of slices
#define K 10 //k shortest path
#define TABLE_PATH "ONE_v1.csv"
bool DEBUG = false;

void setVLinkIDsInProtectedPathsOfVN(vector<vector<int>> vnLinks, protectedPathsOfVN_C* VNPaths){
	for (int i = 0; i < vnLinks.size(); i++) {
		VNPaths->setVLinkID(to_string(vnLinks[i][2]), to_string(vnLinks[i][1]), vnLinks[i][0]);
	}
}

int main(int argc, char * argv[]){
// PARSE ARGUMENTS
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
      } else if (argument.first == "--debug") {
            DEBUG = true;
            printPy("DEBUG Mode is Enabled\n", DEBUG);
      } else {
          printf("Invalid command line option: %s\n", argument.first.c_str());
          return 1;
      }
  }
  string paths_file = vn_topology_filename + ".path";
  string protected_path_output_file = vn_topology_filename + "_protected_path.txt";

  printPy(paths_file + "\n", DEBUG);
  printPy(vn_topology_filename + "\n", DEBUG);
  printPy(location_constraint_filename + "\n", DEBUG);
  printPy(protected_path_output_file + "\n", DEBUG);

//PREPARING INITIAL DATA
  problem_spec vn(paths_file, vn_topology_filename, location_constraint_filename);
  convertor convertor_instance(K, Q);
  reach_table reach_table_instance(TABLE_PATH, Q, K);
	protectedPathsOfVN_C* VNPaths = new protectedPathsOfVN_C(paths_file, 4, 5);
	setVLinkIDsInProtectedPathsOfVN(vn.vlinks, VNPaths);

//WRITING DISJOINT PATHS DATA TO FILE FOR FUTURE USE
	VNPaths->printProtectedPathsOfVN(protected_path_output_file);
	VNPaths->printStats();

//SOLVING VLINKS
	double cost = 0;
  vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> path_slices(vn.shortest_paths.size(), bitset<NUMBER_OF_PHYSICAL_SLICES> (0));
	for (int i = 0; i < path_slices.size(); i++) {
			cout << path_slices[i] << endl;
	}

  for(int vlink_it = 0; vlink_it < vn.vlink_order.size(); vlink_it++) {

    int vlink_index = vn.vlink_order[vlink_it];
    int vlink_id = vn.vlinks[vlink_index][0];
    int bit_rate = vn.vlinks[vlink_index][3];
    protectedPathsOfPair_S vlink_paths = VNPaths->getPathsOfVLink(vlink_id);
    vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> current_vlink_path_slices;
		vector<vector<int>> path_degrees;
		vector<vector<int>> adj_list;
    int starting_path_index = vn.vlink_shortest_paths_indexes[vlink_index][2];
    int ending_path_index = vn.vlink_shortest_paths_indexes[vlink_index][3];
    for(int j = starting_path_index; j < ending_path_index; j++){
      current_vlink_path_slices.push_back(path_slices[j]);
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

		vector<int> next_demands;
		for(int j = vlink_it+1; j < vn.vlink_order.size(); j++) {
				int index = vn.vlink_order[j];
				int demand = vn.vlinks[index][3];
				next_demands.push_back(demand);
		}

    split_option_finder solver(DEBUG, Q, bit_rate, current_vlink_path_slices,
                              vlink_paths, &convertor_instance, &reach_table_instance, path_degrees, next_demands, adj_list);
//		break;

		solution* vnSol = solver.get_solution();

		if(vnSol->get_needed_slices() == -1) {
				cout << "NO Solution ----> " << vlink_it <<"/"<<vn.vlink_order.size()<< endl;
//				return 0;
		}

		cost += vnSol->get_needed_slices();
		cost += vnSol->get_paths().size() * 0.1;


		vector<int> paths = vnSol->get_paths();
		vector<vector<int>> starting_slices = vnSol->get_starting_slices();
		vector<vector<int>> slices_req = vnSol->get_req_slices();

		for(int i = 0; i < paths.size(); i++) {
				int path_index = paths[i] + starting_path_index;
				for (int j = 0; j < starting_slices[i].size(); j++) {
						for(int k = 0; k < slices_req[i][j]; k++){
								path_slices[path_index].set(starting_slices[i][j] + k);

								for (int l = 0; l < vn.adjacency_list[path_index].size(); l++)
										path_slices[vn.adjacency_list[path_index][l]].set(starting_slices[i][j] + k);

						}
				}
		}

//		for (int i = 0; i < path_slices.size(); i++) {
//				cout << path_slices[i] << endl;
//		}
//    vector<vector<int>> adj_list = {};
//    vector<vector<int>> path_degrees = {};
//    vector<int> next_demands = {};
//    vector<int> distances;
//    vector<int> number_of_hops;
    //1
        //FOR EACH PATH OF VLink
//        number_of_hops.push_back(vn.shortest_paths[j][6]);
//        distances.push_back(vn.shortest_paths[j][7]);
//        path_degrees.push_back(vn.path_degree_to_next_vlinks[j]);
//        vector<int>::iterator lb_adj_index = lower_bound(vn.adjacency_list[j].begin(), vn.adjacency_list[j].end(), starting_path_index);
//        vector<int>::iterator ub_adj_index = upper_bound(vn.adjacency_list[j].begin(), vn.adjacency_list[j].end(), ending_path_index);
//        vector<int> current_path_adj_list(lb_adj_index, ub_adj_index);
//        adj_list.push_back(vector<int>(0));
//        for(int k = 0; k < current_path_adj_list.size(); k++)
//            adj_list[adj_list.size()-1].push_back(current_path_adj_list[k]-starting_path_index);
//    }

  }
	cout<<cost<<endl;

  return 0;
}
