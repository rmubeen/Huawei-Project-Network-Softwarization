//
// Created by sepehr on 07/09/18.
//

#include "split_option_finder.h"

bool split_option_finder::is_available(int path_num, int start, int num){
    for(int i = 0; i < num; i++){
      if(path_slices[path_num].test(start+i) == true)
          return false;
    }
    return true;
}

int split_option_finder::enough_space(int needed_slices, bitset<NUMBER_OF_PHYSICAL_SLICES> p) {
    int consecutive_empty_slices = 0;
    for(int i = 0; i < NUMBER_OF_PHYSICAL_SLICES; i++) {
        int begining_index = i;
        while(i < NUMBER_OF_PHYSICAL_SLICES && p.test(i) == false && needed_slices > consecutive_empty_slices) {
            consecutive_empty_slices++;
            i++;
        }
        if(consecutive_empty_slices >= needed_slices)
            return begining_index;
        consecutive_empty_slices = 0;
    }
    return -1;
}

vector<pair<int, int>> split_option_finder::divide_and_merge(vector<int> division, vector<vector<int>> paths){
  vector<pair<int, int>> bwDiv;
	int allocatedBW;
	int tempInt=0;

	for (int i = 0; i < paths.size(); i++) {
    int allocatedBW_a = ceil((double)division[i]/(paths[i].size()));
    int allocatedBW_b = ceil(((double)division[i]/(paths[i].size()-1))*this->bsr_value);
		allocatedBW = max(allocatedBW_a, allocatedBW_b);
    printPy(allocatedBW_a, debug); printPy(" <- BW Case A\n", debug);
    printPy(allocatedBW_b, debug); printPy(" <- BW Case B\n", debug);
    printPy(allocatedBW, debug); printPy(" <- BW Final. (B, n): ", debug); printPy(division[i], debug); printPy(" ", debug); printPy(paths[i].size(), debug); printPy("\n", debug);

		for (int j = 0; j < paths[i].size(); j++) {
			tempInt = findPairInVector(bwDiv, paths[i][j]);
			if (tempInt > -1)
				bwDiv[tempInt].second = (bwDiv[tempInt].second) + allocatedBW;
			else
				bwDiv.push_back(make_pair(paths[i][j], allocatedBW));
		}
	}

  vector<int> all_bit_rates = reach_table_instance->get_all_bit_rates_vector();
  sort(all_bit_rates.begin(), all_bit_rates.end());
  for (int i = 0; i < bwDiv.size(); i++) {
    if(find(all_bit_rates.begin(), all_bit_rates.end(), bwDiv[i].second) == all_bit_rates.end()){
      bwDiv[i].second = all_bit_rates[findIndexOfNextInVector(all_bit_rates, bwDiv[i].second)];
    }
  }

	return bwDiv;
}

vector<pair<int, vector<int>>> split_option_finder::calculateFinalBWDiv(vector<pair<int, int>> paths_bw, vector<int> split_comb, vector<int> bit_rates){
    vector<pair<int, vector<int>>> result;
    for (int path_it = 0; path_it < paths_bw.size(); path_it++) {
        vector<vector<int>> bw_div = intPermutationsHelper(paths_bw[path_it].second, bit_rates, split_comb[path_it]);
        if(bw_div.size() == 0)
          return {};
        result.push_back(make_pair(paths_bw[path_it].first, bw_div[0]));
    }
    return result;
}

bool split_option_finder::solution_feasible(vector<pair<int, vector<int>>> solution) {
    vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> temp_path_slices = this->path_slices;
    printPy("Inter Slices: ", debug); printVector(temp_path_slices, debug);

    for(int path_it = 0; path_it < solution.size(); path_it++){
        for (int tuple_it = 0; tuple_it < solution[path_it].second.size(); tuple_it++) {
            int slices_req = this->reach_table_instance->get_tuple_needed_slices(solution[path_it].second[tuple_it]);
            int start = this->enough_space(slices_req, temp_path_slices[solution[path_it].first]);
            if (start == -1){
                printPy("Not enough space available on path: ", debug); printPy(slices_req, debug); printPy(" ", debug); printPy(temp_path_slices[solution[path_it].first], debug); printPy("\n", debug);
                return false;
            }
            for (int adj_list_it = 0; adj_list_it < adj_list[solution[path_it].first].size(); adj_list_it++) {
                if(!(this->is_available(adj_list[solution[path_it].first][adj_list_it], start, slices_req))){
                    printPy("Not enough space on adjacent paths: ", debug); printPy(start, debug); printPy(" ", debug); printPy(slices_req, debug); printPy(" ", debug); printPy(adj_list[solution[path_it].first][adj_list_it], debug); printPy(" ", debug); printPy(temp_path_slices[adj_list[solution[path_it].first][adj_list_it]], debug); printPy("\n", debug);
                    return false;
                }
            }
            for (int i = 0; i < slices_req; i++) {
                temp_path_slices[solution[path_it].first].set(start+i);
                for (int adj_list_it = 0; adj_list_it < adj_list[solution[path_it].first].size(); adj_list_it++)
                    temp_path_slices[adj_list[solution[path_it].first][adj_list_it]].set(start+i);
            }
        }
    }

    return true;
}

vector<pair<int, vector<int>>> split_option_finder::solveProtectedPaths(protectedPathsOfPair_S allPaths, vector<vector<int>> paths, int index_of_path, int* numOfSlices) {
    printPy("Path to be Solved:\n", debug); print2DVector(paths, debug);
    int size_of_path = paths.size();
    int number_of_paths = allPaths.paths.size();
    vector<int> bitRatesVector = reach_table_instance->get_all_bit_rates_vector();
    sort(bitRatesVector.begin(), bitRatesVector.end());
    vector<pair<int, vector<int>>> solution = {};
    int slices = -1;

    vector<vector<int>> all_divisions = intPermutationsHelper(bit_rate, reach_table_instance->get_all_bit_rates_vector(), size_of_path);
    for (int all_division_it = 0; all_division_it < all_divisions.size(); all_division_it++) {
      //FOR EACH DIVISION
      printPy("Division: ", debug); printVector(all_divisions[all_division_it], debug);

        vector<pair<int, int>> paths_bw = divide_and_merge(all_divisions[all_division_it], paths);
        for(int number_of_splits = paths_bw.size(); number_of_splits <= max_number_of_splits; number_of_splits++) {
        //FOR EACH POSSIBLE q <= Q
            vector<vector<int>> all_split_combinations = intPermutationsHelper(number_of_splits, {}, paths_bw.size());
            for (int split_comb_it = 0; split_comb_it < all_split_combinations.size(); split_comb_it++) {
                printPy("Split Comb: ", debug); printVector(all_split_combinations[split_comb_it], debug);
                vector<pair<int, vector<int>>> final_bw_div = this->calculateFinalBWDiv(paths_bw, all_split_combinations[split_comb_it], bitRatesVector);
                vector<pair<int, vector<int>>> tempSolution = {};
                int tempSlices = 0;
                bool status = true;
                if(final_bw_div.size() == 0){
                    printPy("Final BW division not found.\n", debug);
                    status = false;
//                    break;
                }
                for (int path_it = 0; (path_it < final_bw_div.size()) && status; path_it++) {
//                    printPy("entered path iterator\n", debug);
                    vector<int> tupleIndces = {};
                    for (int i = 0; i < final_bw_div[path_it].second.size(); i++) {
                        int path_dist = allPaths.paths[final_bw_div[path_it].first].dist;
                        vector<int> tuple = reach_table_instance->get_best_tuple(path_dist, final_bw_div[path_it].second[i]);
                        if (tuple.size() == 0){
                            printPy("Valid tuple for path not found-> ", debug); printPy(final_bw_div[path_it].first, debug); printPy(" ", debug); printPy(path_dist, debug); printPy(" ", debug); printPy(final_bw_div[path_it].second[i], debug);
                            status = false;
                            break;
                        }
//                        printVector(tuple);
                        tupleIndces.push_back(tuple[0]);
                        tempSlices += tuple[3];
                    }
                    tempSolution.push_back(make_pair(final_bw_div[path_it].first, tupleIndces));
                }
                if(status){
                    if (slices == -1 || tempSlices < slices) {
                        printPy("found a candidate solution\n", debug);
                        if (this->solution_feasible(tempSolution)){
                          printPy("***************new solution found! updating...\n", debug);
                          solution = tempSolution;
                          slices = tempSlices;
//                          printPy("Naaaaaaaaaaaaaaaaaaaaaaaaa\n");
                        }
                    }
                }
            }
        }
    }
    printPy(slices, debug);
    printPy(" <-slices required for this path\n", debug);
    *numOfSlices = slices;
    return solution;
/*
    if((slices != -1) && (slices != 99999999)){
        printPy("Paths\n");
        print2DVector(paths);
        printPy("\nSlices Required: ");
        printPy(slices);
        printPy("\n\n");
    }
*/
}

split_option_finder::split_option_finder(bool DBG, double bsr_value, int Q, int bit_rate, vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> slices,
                protectedPathsOfPair_S paths, convertor *convertor_ins, reach_table *reach_table_ins, vector<vector<int>> path_degrees, vector<int> next_demands, vector<vector<int>> adj_list){

  printPy("\n-------------> ENTERING INTO SOLVER\n", DBG);
  this->bsr_value = bsr_value;
  printPy(this->bsr_value, DBG); printPy(" <- bsr value\n", DBG);
  this->bit_rate = bit_rate;
  printPy(bit_rate, DBG); printPy(" <- bit rates\n", DBG);
  this->number_of_paths = paths.paths.size();
  this->max_number_of_splits = Q;
  printPy(Q, DBG); printPy(" <- Q\n", DBG);
  this->path_slices = slices;
  printPy("Initial Slices: ", DBG); printVector(slices, DBG);
  this->convertor_instance = convertor_ins;
  this->reach_table_instance = reach_table_ins;
  this->allVNPaths = paths;
  printPy(paths.protectedPathsSet.size(), DBG); printPy(" <- # of paths\n", DBG);
  this->path_degrees = path_degrees;
  this->next_demands = next_demands;
  this->adj_list = adj_list;
  printPy("adj list:\n", DBG); print2DVector(adj_list, DBG);
  this->min_slices = -1;
  this->debug = DBG;
  vector<pair<int, vector<int>>> cur_sol = {};
  int* numOfSlices = new int(0);

    for(int PPSet_it = 0; PPSet_it < paths.protectedPathsSet.size(); PPSet_it++){
        vector<pair<int, vector<int>>> tempSolution = this->solveProtectedPaths(paths, paths.protectedPathsSet[PPSet_it], PPSet_it, numOfSlices);
//        printPy("Inter Slices: ", DBG); printVector(slices, DBG);
        if (this->min_slices == -1 || ((*numOfSlices < this->min_slices) && (*numOfSlices != -1))) {
            cur_sol = tempSolution;
            this->min_slices = *numOfSlices;
        }
    }

    vector<int> sol_paths;
    vector<vector<int>> slices_req;
    vector<vector<int>> slices_start;
    vector<vector<int>> tuples;
    vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> temp_path_slices = this->path_slices;

    for(int path_it = 0; path_it < cur_sol.size(); path_it++){
        sol_paths.push_back(cur_sol[path_it].first);
        slices_req.push_back({});
        slices_start.push_back({});
        tuples.push_back({});

        for (int tuple_it = 0; tuple_it < cur_sol[path_it].second.size(); tuple_it++) {
            tuples[path_it].push_back(cur_sol[path_it].second[tuple_it]);
            int slice_req = this->reach_table_instance->get_tuple_needed_slices(cur_sol[path_it].second[tuple_it]);
            slices_req[path_it].push_back(slice_req);
            int start = this->enough_space(slice_req, temp_path_slices[cur_sol[path_it].first]);
            slices_start[path_it].push_back(start);
        }
    }

    if(this->min_slices == -1)
        this->vnSolutin = new solution();
    else
        this->vnSolutin = new solution(sol_paths, tuples, slices_start, slices_req, path_degrees, next_demands, paths);
     printPy("solution slices: ", DBG); printPy(this->min_slices, debug); printPy("\n", debug);
}
