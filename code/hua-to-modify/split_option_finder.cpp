//
// Created by sepehr on 07/09/18.
//

#include "split_option_finder.h"
#include "basic_functions.cpp"

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

solution split_option_finder::number_of_slices_needed_base_cases(int path_id, vector<vector<int>> best_tuples, int current_solution_obj) {
//    cout << "CP4\n";

    int needed_slices = 0;
    int number_of_tuples = best_tuples.size();

    for(int i = 0; i < number_of_tuples; i++)
        needed_slices += best_tuples[i][3];

    needed_slices *= allVNPaths.paths[path_id].hopes;

    if(current_solution_obj != -1 && needed_slices >= current_solution_obj)
        return solution(); //return an invalid solution
//    cout << "CP4.2\n";

    vector<vector<int>> all_permuations = reach_table_instance->get_all_permutations(number_of_tuples);
    int *begining_slices = new int[number_of_tuples];
    for(int i=0; i<all_permuations.size(); i++) {
        bitset<NUMBER_OF_PHYSICAL_SLICES> path_current_state = path_slices[path_id];
        bool flag = true;
        for(int j=0; j<all_permuations[i].size(); j++) {
            int tuple_j_needed_slices = best_tuples[all_permuations[i][j]][3];
            int starting_index_for_current_tuple = enough_space(tuple_j_needed_slices, path_current_state);
            if(starting_index_for_current_tuple == -1) {
                flag = false;
                break;
            }
            for(int s = 0; s <tuple_j_needed_slices; s++)
                path_current_state.set(s+starting_index_for_current_tuple);
            begining_slices[all_permuations[i][j]] = starting_index_for_current_tuple;
        }
//        cout << "CP4.4\n";
        if(flag) {
            vector<int> solution_path(number_of_tuples, path_id);
            vector<int> solution_tuple_id;
            for(int l = 0; l < number_of_tuples; l++)
                solution_tuple_id.push_back(best_tuples[l][0]);
//            cout << "CP4.6\n";

//            return solution(solution_path, solution_tuple_id, vector<int>(begining_slices, begining_slices+number_of_tuples), needed_slices, path_degrees, reach_table_instance, next_demands);
        }
    }
//    cout << "CP4.8\n";
    return solution(); //invalid solution
}

solution split_option_finder::find_solution(vector<int> path_ids, vector<int> tuple_ids) {
    int number_of_tuples = tuple_ids.size();
    vector<int> tuples_needed_slices(0);
    int needed_slices = 0;
    for(int i=0; i<tuple_ids.size(); i++) {
        tuples_needed_slices.push_back(reach_table_instance->get_tuple_needed_slices(tuple_ids[i]));
        needed_slices += tuples_needed_slices[i] * number_of_hops[path_ids[i]];
    }
    vector<vector<int>> all_permuations = reach_table_instance->get_all_permutations(number_of_tuples);
    int *begining_slices=new int[number_of_tuples];

    for(int i=0; i<all_permuations.size(); i++) {
        vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> path_current_state = path_slices;
        bool flag = true;
        for(int j=0; j<all_permuations[i].size(); j++) {
            int tuple_j_needed_slices = tuples_needed_slices[all_permuations[i][j]];

            int starting_index_for_current_tuple = enough_space(tuple_j_needed_slices, path_current_state[path_ids[all_permuations[i][j]]]);
            if(starting_index_for_current_tuple == -1) {
                flag = false;
                break;
            }
            for(int s = 0; s < tuple_j_needed_slices; s++)
                path_current_state[path_ids[all_permuations[i][j]]].set(s+starting_index_for_current_tuple);
            for(int adj_index = 0; adj_index < adj_list[path_ids[all_permuations[i][j]]].size(); adj_index++) {
                for(int s = 0; s < tuple_j_needed_slices; s++)
                    path_current_state[adj_list[path_ids[all_permuations[i][j]]][adj_index]].set(s+starting_index_for_current_tuple);
            }
            begining_slices[all_permuations[i][j]] = starting_index_for_current_tuple;
        }
//        if(flag)
//            return solution(path_ids, tuple_ids, vector<int>(begining_slices, begining_slices + number_of_tuples),
//                            needed_slices, path_degrees, reach_table_instance, next_demands);
    }
    return solution();
}

vector<pair<int, int>> split_option_finder::divide_and_merge(vector<int> division, vector<vector<int>> paths){
  vector<pair<int, int>> bwDiv;
	int allocatedBW;
	int tempInt=0;

	for (int i = 0; i < paths.size(); i++) {
		allocatedBW = division[i]/(paths[i].size()-1);

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

    for(int path_it = 0; path_it < solution.size(); path_it++){
        for (int tuple_it = 0; tuple_it < solution[path_it].second.size(); tuple_it++) {
            int slices_req = this->reach_table_instance->get_tuple_needed_slices(solution[path_it].second[tuple_it]);
            int start = this->enough_space(slices_req, temp_path_slices[solution[path_it].first]);
            if (start == -1)
                return false;
            for (int adj_list_it = 0; adj_list_it < adj_list[solution[path_it].first].size(); adj_list_it++) {
                if(!this->is_available(adj_list[solution[path_it].first][adj_list_it], start, slices_req))
                    return false;
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
    int size_of_path = paths.size();
    int number_of_paths = allPaths.paths.size();
    vector<int> bitRatesVector = reach_table_instance->get_all_bit_rates_vector();
    sort(bitRatesVector.begin(), bitRatesVector.end());
    vector<pair<int, vector<int>>> solution = {};
    int slices = -1;

//    cout << bit_rate << endl;
//    cout << reach_table_instance->get_bit_rate_index(bit_rate) << endl;
//    cout << size_of_path << endl;
//    vector<vector<int>> all_divisions = reach_table_instance->get_all_bit_rate_combination_with_specific_sum(reach_table_instance->get_bit_rate_index(bit_rate), size_of_path);
//    cout << "here\n";
    vector<vector<int>> all_divisions = intPermutationsHelper(bit_rate, reach_table_instance->get_all_bit_rates_vector(), size_of_path);


    for (int all_division_it = 0; all_division_it < all_divisions.size(); all_division_it++) {
      //FOR EACH DIVISION
//      printPy("Division\n");
//      printVector(all_divisions[all_division_it]);

        vector<pair<int, int>> paths_bw = divide_and_merge(all_divisions[all_division_it], paths);
        for(int number_of_splits = paths_bw.size(); number_of_splits <= max_number_of_splits; number_of_splits++) {
        //FOR EACH POSSIBLE Q
            vector<vector<int>> all_split_combinations = intPermutationsHelper(number_of_splits, {}, paths_bw.size());
            for (int split_comb_it = 0; split_comb_it < all_split_combinations.size(); split_comb_it++) {
                vector<pair<int, vector<int>>> final_bw_div = this->calculateFinalBWDiv(paths_bw, all_split_combinations[split_comb_it], bitRatesVector);
                vector<pair<int, vector<int>>> tempSolution = {};
                int tempSlices = 0;
                bool status = true;
                if(final_bw_div.size() == 0){
                    status = false;
                    break;
                }
                for (int path_it = 0; (path_it < final_bw_div.size()) && status; path_it++) {
                    vector<int> tupleIndces = {};
                    for (int i = 0; i < final_bw_div[path_it].second.size(); i++) {
                        int path_dist = allPaths.paths[final_bw_div[path_it].first].dist;
                        vector<int> tuple = reach_table_instance->get_best_tuple(path_dist, final_bw_div[path_it].second[i]);
                        if (tuple.size() == 0){
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
                        if (this->solution_feasible(tempSolution)){
                          solution = tempSolution;
                          slices = tempSlices;
//                          printPy("Naaaaaaaaaaaaaaaaaaaaaaaaa\n");
                        }
                    }
                }
            }
        }
    }
//    printPy(slices);
//    printPy(" slices required\n");
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

void split_option_finder::solve_dp() {
    // base cases
    vector<int> bit_rates = reach_table_instance->get_all_bit_rates_vector();
    for(int bit_rate_index = 0; bit_rate_index < bit_rates.size() && bit_rates[bit_rate_index] <= bit_rate; bit_rate_index++) {
      // FOR EACH VALID BIT RATE
        for(int number_of_splits = 1; number_of_splits <= max_number_of_splits; number_of_splits++) {
          //FOR EACH VALID SPLIT
//            cout << "CP1\n";
            vector<vector<int>> all_bit_rate_combinations = reach_table_instance->get_all_bit_rate_combination_with_specific_sum(bit_rate_index, number_of_splits);
            for(int p = 0; p < number_of_paths; p++) {
              //FOR EACH PATH
                vector<int> current_combination(0);
                for(int i = 0; i < number_of_paths+1; i++)
                    current_combination.push_back(0);
                current_combination[p] = number_of_splits;
                current_combination[number_of_paths] = max_number_of_splits - number_of_splits;

                int current_combination_index = convertor_instance->get_combination_index(current_combination);
//                cout << "CP2\n";

                int current_solution_value = -1;

                for(int i = 0; i < all_bit_rate_combinations.size(); i++) {
                  // FOR ALL BIT RATE COMBINATIONS
                    vector<vector<int>> best_tuples(0);
                    bool possible = true;
                    for(int j = 0; j < number_of_splits; j++) {

//                        cout << "CP2.1\n";
                        vector<int> tuple = reach_table_instance->get_best_tuple(allVNPaths.paths[p].dist, all_bit_rate_combinations[i][j]); // optimization
//                        cout << "CP2.2\n";
                        if(tuple.size() == 0) {
                            possible = false;
                            break;
                        }

                        best_tuples.push_back(tuple);
                    }
  //                  cout << "CP3\n";
                    if(possible) {
  //                      cout << "CP3.1\n";
                        solution new_solution = number_of_slices_needed_base_cases(p, best_tuples, current_solution_value);
  //                      cout << "CP3.2\n";
                        if(new_solution.get_needed_slices() != -1) {
                            dynamic_table[bit_rate_index][current_combination_index] = new_solution;
                            current_solution_value = new_solution.get_needed_slices();
                            cout << bit_rate_index << " \t" << current_combination_index << " \t" << current_solution_value << endl;

                        }
//                        cout << "CP3.3\n";
                    }
                }
            }
        }
    }

/*
    cout << "non-base cases\n";

    int number_of_combinations = convertor_instance->get_number_of_combinations();
    vector<vector<int>> **all_sum_combinations = reach_table_instance->get_all_sum_combinations();

//    printPy("all sum combinations\n"); //printPy(all_sum_combinations);
//    reach_table_instance->print_sum_combinations();

    for(int i = 0; i < number_of_combinations; i++) {
        cout << "CP-NBC @ 1\n";
        int combination_value = convertor_instance->get_combination_value_by_index(i);
        vector<int> combination = convertor_instance->int_to_combination(combination_value);

        if(convertor_instance->is_base_combination(combination))
            continue;

        int current_combination_index = i;

        int number_of_distincts_path = convertor_instance->get_number_of_distinct_members(combination);
        vector<vector<int>> parent_cases = convertor_instance->get_parent_cases(combination_value);
        vector<int> parents_combination_index(0);
        for(int j = 0; j < parent_cases.size(); j++) {
            parents_combination_index.push_back(convertor_instance->get_combination_index(parent_cases[j]));
        }
        cout << "CP-NBC @ 3\n";
        vector<int> this_comb_path_ids = convertor_instance->get_combination_path_ids(combination);

        for(int bit_rate_index = 0; bit_rate_index < bit_rates.size() && bit_rates[bit_rate_index] <= bit_rate; bit_rate_index++) {
            int current_solution_value = dynamic_table[bit_rate_index][current_combination_index].get_needed_slices();
            cout << "CP-NBC @ 4\n";

            for(int sum_comb_index = 0; sum_comb_index < all_sum_combinations[bit_rate_index][number_of_distincts_path].size(); sum_comb_index++) {
                vector<int> this_comb_tuple_ids(0);
                bool flag = true;
                cout << "CP-NBC @ 5\n";
                int needed_slices_for_this_sum_combination = 0;
                for(int parent_id = 0; parent_id < number_of_distincts_path; parent_id++) {
                    int parent_bit_rate = all_sum_combinations[bit_rate_index][number_of_distincts_path][sum_comb_index][parent_id];
                    solution this_parent_solution = dynamic_table[reach_table_instance->get_bit_rate_index(parent_bit_rate)][parents_combination_index[parent_id]];
                    if(this_parent_solution.get_needed_slices() == -1) {
                        flag = false;
                        break;
                    }
                    vector<int> this_solution_tuples = this_parent_solution.get_tuples();
                    needed_slices_for_this_sum_combination += this_parent_solution.get_needed_slices();
                    for(int tuple_id = 0; tuple_id < this_solution_tuples.size(); tuple_id++)
                        this_comb_tuple_ids.push_back(this_solution_tuples[tuple_id]);
                }
                cout << "CP-NBC @ 6\n";
                if(flag && (current_solution_value == -1 || current_solution_value > needed_slices_for_this_sum_combination)) {
                    solution solution_using_sum_combiantion = find_solution(this_comb_path_ids, this_comb_tuple_ids);
                    if(solution_using_sum_combiantion.get_needed_slices() != -1) {
                        cout << bit_rate_index << "  " << current_combination_index << "CP-NBC @ 6\n";
                        dynamic_table[bit_rate_index][current_combination_index] = solution_using_sum_combiantion;
                        current_solution_value = solution_using_sum_combiantion.get_needed_slices();
                    }
                }
                cout << "CP-NBC @ 7\n";
            }
        }
    }

    // going through all the combination and find the valid solutions for the vlink
/*    for(int i = 0; i < number_of_combinations; i++)
        if(dynamic_table[reach_table_instance->get_bit_rate_index(bit_rate)][i].get_needed_slices() != -1) {
            solution_array.push_back(dynamic_table[reach_table_instance->get_bit_rate_index(bit_rate)][i]);
        }

    sort(solution_array.begin(), solution_array.begin()+solution_array.size());
*/
}

split_option_finder::split_option_finder(bool DBG, int Q, int bit_rate, vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> slices,
                protectedPathsOfPair_S paths, convertor *convertor_ins, reach_table *reach_table_ins, vector<vector<int>> path_degrees, vector<int> next_demands, vector<vector<int>> adj_list){
  this->bit_rate = bit_rate;
  this->number_of_paths = paths.paths.size();
  this->max_number_of_splits = Q;
  this->path_slices = slices;
  this->convertor_instance = convertor_ins;
  this->reach_table_instance = reach_table_ins;
  this->allVNPaths = paths;
  this->path_degrees = path_degrees;
  this->next_demands = next_demands;
  this->adj_list = adj_list;
  this->min_slices = -1;

  int number_of_bit_rates = reach_table_instance->get_number_of_bit_rates();
  int number_of_combinations = convertor_instance->get_number_of_combinations();
  dynamic_table = new solution*[number_of_bit_rates];
  for(int i=0; i<number_of_bit_rates; i++)
      dynamic_table[i] = new solution[number_of_combinations];

  vector<pair<int, vector<int>>> cur_sol = {};
  int* numOfSlices = new int(0);
  printPy("Completed var declarations @ split option finder\n", DBG);

//  solve_dp();

//  printPy("Completed base combintation @ split option finder\n", DBG);

    for(int PPSet_it = 0; PPSet_it < paths.protectedPathsSet.size(); PPSet_it++){
        vector<pair<int, vector<int>>> tempSolution = this->solveProtectedPaths(paths, paths.protectedPathsSet[PPSet_it], PPSet_it, numOfSlices);
        if (this->min_slices == -1 || ((*numOfSlices < this->min_slices) && (*numOfSlices != -1))) {
            cur_sol = tempSolution;
            this->min_slices = *numOfSlices;
        }
    }
//    printPy(PPSet_it); printPy(" "); printPy(paths.protectedPathsSet.size()); printPy("\n");
//    printPy("Completed a PP Set @ split option finder concerter\n", DBG);

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
        this->vnSolutin = new solution(sol_paths, tuples, slices_start, slices_req, path_degrees, next_demands);
    printPy(this->min_slices);
    printPy("\n");
}

vector<solution> split_option_finder::get_solutions_vector() {
    return solution_array;
}

void split_option_finder::print() {
    for(int i=0; i<path_slices.size(); i++) {
        cout<<path_slices[i].to_string<char, string::traits_type, string::allocator_type>()<<" ";
    }
    cout<<endl;
    for(int i=0; i<number_of_hops.size(); i++) {
        cout<<number_of_hops[i]<<" ";
    }
    cout<<endl;
    for(int i =0; i<path_distances.size(); i++) {
        cout<<path_distances[i]<<" ";
    }
    cout<<endl;
}
void split_option_finder::print_solutions() {}
  /*
    for(int i=0; i<solution_array.size(); i++) {
        solution x = solution_array[i];
        int needed_slices = x.get_needed_slices();
        vector<int> path_ids = x.get_paths();
//        vector<int> tuple_ids = x.get_tuples();
//        vector<int> starting_slice_for_each_tuple = x.get_starting_slices();

        cout<<"solution["<<i<<"]: "<<endl;
        cout<<"needed slices: "<<needed_slices;
        cout<<" cost: "<<x.get_cost();

        cout<<" paths: {";
        for(int j=0; j<path_ids.size(); j++) {
            cout << path_ids[j];
            if(j != path_ids.size()-1)
                cout<<", ";
        }
        cout<<"}"<<endl;

        cout<<"tuples: {";
        for(int j=0; j<tuple_ids.size(); j++) {
            cout << tuple_ids[j];
            if(j != tuple_ids.size()-1)
                cout<<", ";
        }
        cout<<"}"<<endl;

        cout<<"starting slice: {";
        for(int j=0; j<starting_slice_for_each_tuple.size(); j++) {
            cout << starting_slice_for_each_tuple[j];
            if(j != starting_slice_for_each_tuple.size()-1)
                cout<<", ";
        }
        cout<<"}"<<endl<<endl;

    }

}
*/
