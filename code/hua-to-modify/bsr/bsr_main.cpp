#include <iostream>
#include <fstream>
#include <bitset>
#include <ctime>
#include <fstream>
#include "io.h"
#include "basic_functions.cpp"
#include "problem_spec.cpp"
#include "split_option_finder.cpp"
#include "solution.h"
#include "convertor.cpp"
#include "reach_table.cpp"
#include "hua_disjoint_paths.cpp"
using namespace std;

#define Q 8 //max number of slices
#define K 25 //k shortest path
#define TABLE_PATH "input/ONE_v1.csv"

void setVLinkIDsInProtectedPathsOfVN(vector<vector<int>> vnLinks, protectedPathsOfVN_C* VNPaths){
	for (int i = 0; i < vnLinks.size(); i++) {
		VNPaths->setVLinkID(to_string(vnLinks[i][2]), to_string(vnLinks[i][1]), vnLinks[i][0]);
	}
}

int main(int argc, char * argv[]){
	//PARSING ARGUMENTS//
	time_t t0 = time(NULL);
  auto arg_map = ParseArgs(argc, argv);
	bool DEBUG = false;
	bool allDebug = false;
  string pn_topology_filename = "";
  string vn_topology_filename = "";
  string location_constraint_filename = "";
	string output_filename = "";
	double bsr_value = 1.0;
  for (auto argument : *arg_map) {
      if (argument.first == "--pn_topology_file") {
          pn_topology_filename = argument.second;
      } else if (argument.first == "--vn_topology_file") {
          vn_topology_filename = argument.second;
      } else if (argument.first == "--location_constraint_file") {
          location_constraint_filename = argument.second;
			} else if (argument.first == "--output") {
          output_filename = argument.second;
      } else if (argument.first == "--debug") {
						if (argument.second == "main") {
								DEBUG = true;
								printPy("\nDEBUG MODE IS ENABLED FOR MAIN SCRIPT.\n\n", DEBUG);
						} else if(argument.second == "all"){
								DEBUG = true;
								allDebug = true;
								printPy("\nDEBUG MODE IS ENABLED FOR ALL SCRIPTS.\n\n", DEBUG && allDebug);
						}
			} else if (argument.first == "--bsr") {
						bsr_value = (double)strToInt(argument.second)/100.0;
      } else {
          printf("Invalid command line option: %s\n", argument.first.c_str());
          return 1;
      }
  }
  string paths_file = vn_topology_filename + ".path";
  string protected_path_output_file = vn_topology_filename + "_protected_path.txt";

  printPy(paths_file + " \t<- path\n", DEBUG);
  printPy(vn_topology_filename + " \t<- topology\n", DEBUG);
  printPy(location_constraint_filename + " \t<- constraint\n", DEBUG);
  printPy(protected_path_output_file + " \t<- protected path output\n", DEBUG);
	printPy(output_filename + " \t<- output\n", DEBUG);
	printPy(bsr_value, DEBUG); printPy(" \t<- BSR Value\n", DEBUG);

	//PREPARING INITIAL DATA//
  problem_spec vn(paths_file, vn_topology_filename, location_constraint_filename);
	printPy("Problem Spec VN Defined\n", DEBUG);
//  convertor convertor_instance(K, Q);
	printPy("Converter Instance Defined\n", DEBUG);
  reach_table reach_table_instance(TABLE_PATH, Q, K);
	printPy("Reach Table Instance Defined\n", DEBUG);
	protectedPathsOfVN_C* VNPaths = new protectedPathsOfVN_C(paths_file, 4, 5);
	printPy("VN Disjoint Paths Done\n", DEBUG);
	setVLinkIDsInProtectedPathsOfVN(vn.vlinks, VNPaths);
	printPy("VLinks IDs Set\n", DEBUG);
	//WRITING DISJOINT PATHS DATA TO FILE FOR FUTURE USE//
	VNPaths->printProtectedPathsOfVN(protected_path_output_file);
	printPy("Disjoint Sets Exported\n", DEBUG);
	if (DEBUG)
		VNPaths->printStats();

	//INITIALIZING COST AND PATHS//
	time_t t1 = time(NULL);
	printPy("\n");
	printPy(t1-t0); printPy(" second(s) taken for initialization\n");
	double cost = 0;
  vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> path_slices(vn.shortest_paths.size(), bitset<NUMBER_OF_PHYSICAL_SLICES> (0));

	//SOLVING VLINKS: EACH IN ONE ITERATION//
  for(int vlink_it = 0; vlink_it < vn.vlink_order.size(); vlink_it++) {

		//GETING DATA FOR THIS VN//
    int vlink_index = vn.vlink_order[vlink_it];
    int vlink_id = vn.vlinks[vlink_index][0];
		cout << "\nvlink src/dst: " << vn.vlinks[vlink_index][1] << " " << vn.vlinks[vlink_index][2] << endl;
		printPy("vLink ID: ", DEBUG); printPy(vlink_id, DEBUG); printPy("\n", DEBUG);
    int bit_rate = vn.vlinks[vlink_index][3];
    protectedPathsOfPair_S vlink_paths = VNPaths->getPathsOfVLink(vlink_id);
		cout << "number of protected paths set: " << vlink_paths.protectedPathsSet.size() << endl;
    vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> current_vlink_path_slices;
		vector<vector<int>> path_degrees;
		vector<vector<int>> adj_list;
    int starting_path_index = vn.vlink_shortest_paths_indexes[vlink_index][2];
    int ending_path_index = vn.vlink_shortest_paths_indexes[vlink_index][3];
		printPy(starting_path_index, DEBUG); printPy(" -> ", DEBUG); printPy(ending_path_index, DEBUG); printPy("\n", DEBUG);

    for(int j = starting_path_index; j < ending_path_index; j++){
      current_vlink_path_slices.push_back(path_slices[j]);
			path_degrees.push_back(vn.path_degree_to_next_vlinks[j]);
			vector<int>::iterator lb_adj_index = lower_bound(vn.adjacency_list[j].begin(), vn.adjacency_list[j].end(), starting_path_index);
			vector<int>::iterator ub_adj_index = upper_bound(vn.adjacency_list[j].begin(), vn.adjacency_list[j].end(), ending_path_index-1);
			vector<int> current_path_adj_list(lb_adj_index, ub_adj_index);
			printPy("Adjacency list for: ", DEBUG); printPy(j, DEBUG); printPy(" ", DEBUG); printVector(current_path_adj_list, DEBUG);
			adj_list.push_back(vector<int>(0));
			for(int k = 0; k < current_path_adj_list.size(); k++)
					adj_list[adj_list.size()-1].push_back(current_path_adj_list[k]-starting_path_index);
		}

		vector<int> next_demands;
		for(int j = vlink_it+1; j < vn.vlink_order.size(); j++) {
				int index = vn.vlink_order[j];
				int demand = vn.vlinks[index][3];
				next_demands.push_back(demand);
		}

		//SOLVING THIS VN//
    split_option_finder solver(allDebug, bsr_value, Q, bit_rate, current_vlink_path_slices,
                              vlink_paths, &reach_table_instance, path_degrees, next_demands, adj_list);

		//GETING SOLUTION OF ABOVE SOLVER//
		solution vnSol = solver.get_optimal_solution();
		if(vnSol.get_needed_slices() == -1) {
				cout << "NO Solution ----> " << vlink_it <<"/"<<vn.vlink_order.size()<< endl;
				if (vlink_paths.protectedPathsSet.size() > 0)
						return 1;
		}

		vnSol.print_solution();



		//UPDATING COST VARIABLE//
		cost += vnSol.get_needed_slices();
//		cost += vnSol->get_paths().size() * 0.1;

		//UPDATING PATH SLICES//
		vector<int> paths = vnSol.get_paths();
		vector<vector<int>> starting_slices = vnSol.get_starting_slices();
		vector<vector<int>> slices_req = vnSol.get_req_slices();
//		print2DVector(adj_list);
		for(int i = 0; i < paths.size(); i++) {
				cout << "dist: " << vlink_paths.paths[paths[i]].dist << endl;
				int path_index = paths[i] + starting_path_index;
//				cout << path_index << endl;
//				printVector(vn.adjacency_list[path_index]);
				for (int j = 0; j < starting_slices[i].size(); j++) {
						for(int k = 0; k < slices_req[i][j]; k++){
								path_slices[path_index].set(starting_slices[i][j] + k);

								for (int l = 0; l < adj_list[path_index-starting_path_index].size(); l++){
										path_slices[l+starting_path_index].set(starting_slices[i][j] + k);
								}
						}
				}
		}

		//PRINTING UPDATED PATH SLICES//
		for (int i = starting_path_index; i < ending_path_index && DEBUG; i++) {
				cout << path_slices[i] << endl;
		}
//		break;
  }
	time_t t2 = time(NULL);

	printPy(t2-t1); printPy(" second(s) taken for solution\n");
	printPy(t2-t0); printPy(" second(s) taken in total\n");
	printPy("cost = "); printPy(cost); printPy("\n\n");

	if(output_filename == ""){
			printPy("NO OUTPUT FILE MENTIONED\n\n");
	} else {
			ofstream outfile;
  		outfile.open(output_filename);
  		outfile << "cost = ";
			outfile << cost;
			outfile << endl;
			outfile.close();
	}

  return 0;
}
