//
// Created by sepehr on 07/09/18.
//

#include "split_option_finder.h"

bool split_option_finder::is_available(int start, int num, bitset<NUMBER_OF_PHYSICAL_SLICES> pathSlices){
    for(int i = 0; i < num; i++){
      if(pathSlices.test(start+i) == true)
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
  for (int i = 0; i < bwDiv.size(); i++) {
    if(find(all_bit_rates.begin(), all_bit_rates.end(), bwDiv[i].second) == all_bit_rates.end()){
      bwDiv[i].second = all_bit_rates[findIndexOfNextInVector(all_bit_rates, bwDiv[i].second)];
    }
  }

	return bwDiv;
}

vector<pair<int, vector<vector<int>>> > split_option_finder::calculateFinalBWDiv(vector<pair<int, int>> paths_bw, vector<int> split_comb, vector<int> bit_rates){
    vector<pair<int, vector<vector<int>>> > result = {};
    for (int path_it = 0; path_it < paths_bw.size(); path_it++) {
        vector<vector<int>> bw_div = remove_repition_in_vector(intPermutationsHelper(paths_bw[path_it].second, bit_rates, split_comb[path_it]));
        if(bw_div.size() == 0)
          return {};
        result.push_back(make_pair(paths_bw[path_it].first, bw_div));
    }
    sort(result.begin(), result.end(), [](pair<int, vector<vector<int>>> a, pair<int, vector<vector<int>>> b){
        return a.first < b.first;
    });
    return result;
}

solution split_option_finder::solution_feasible(vector<pair<int, int>> solutionVec, int needed_slices){
    solution result = solution();
    vector<int> paths = {};
    vector<int> tuples = {};
    vector<int> starting_slices_index = {};
    vector<int> slices_req = {};
    vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> pathSlices = this->path_slices;
    printPy("Inter Slices: ", debug); printVector(pathSlices, debug);
    bool flag;

    sort(solutionVec.begin(), solutionVec.end(), [](pair<int, int> first, pair<int, int> sec){
        return first.first < sec.first;
    });

  do{
    paths = {};
    tuples = {};
    slices_req = {};
    starting_slices_index = {};
    pathSlices = this->path_slices;
    flag = true;

    for(int path_it = 0; path_it < solutionVec.size(); path_it++){
        int path_index = solutionVec[path_it].first;
        paths.push_back(path_index);
        tuples.push_back(solutionVec[path_it].second);

        int slice_req = this->reach_table_instance->get_tuple_needed_slices(solutionVec[path_it].second);
        slices_req.push_back(slice_req);

        int start = this->enough_space(slice_req, pathSlices[path_index]);
        if(start == -1){
            flag = false;
            break;
        }
        starting_slices_index.push_back(start);

        for (int i = 0; i < slice_req; i++) {
            pathSlices[path_index].set(start+i);
            for (int adj_list_it = 0; adj_list_it < adj_list[path_index].size(); adj_list_it++){
                pathSlices[adj_list[path_index][adj_list_it]].set(start+i);
            }
        }

    }

    if(flag){
        result = solution(paths, tuples, slices_req, starting_slices_index, needed_slices, this->path_degrees, this->next_demands);
        return result;
    }
  }while(next_permutation(solutionVec.begin(), solutionVec.end(), [](pair<int, int> first, pair<int, int> sec){
      return first.first < sec.first;
    }));

    return result;
}

solution split_option_finder::return_solution(vector<pair<int, vector<int>>> bitrateDiv, int cur_min){
  vector<pair<int, int>> solutionVec;
  solution result = solution();
  int slices_needed = 0;

  for (int path_it = 0; path_it < bitrateDiv.size(); path_it++){
      for (int path_bw_it = 0; path_bw_it < bitrateDiv[path_it].second.size(); path_bw_it++) {
          int bit_rate_index = findIndexOfNextInVector(this->all_bit_rates, bitrateDiv[path_it].second[path_bw_it]);
          vector<int> tuple = this->dynamicTable[bitrateDiv[path_it].first][bit_rate_index][0][0];
          if(tuple.size() == 0){
                  printPy("Valid tuple for path not found-> ", debug); printPy(bitrateDiv[path_it].first, debug); printPy(" ", debug); printPy(bitrateDiv[path_it].second[path_bw_it], debug); printPy("\n", debug);
                  return result;
          }
          slices_needed +=  this->reach_table_instance->get_tuple_needed_slices(tuple[0]) * this->allVNPaths.paths[bitrateDiv[path_it].first].hopes;
          solutionVec.push_back(make_pair(bitrateDiv[path_it].first, tuple[0]));
      }
  }

  if(slices_needed > cur_min)
      return result;

  result = solution_feasible(solutionVec, slices_needed);
  if(debug)
    result.print_solution();
  return result;
}

vector<int> split_option_finder::find_next_comb(vector<int> cur, vector<int> max){
    int size = cur.size();

    for (int i = size-1; i > -1; i--) {
        if(cur[i] == max[i]){
            cur[i] = 0;
        } else {
          cur[i] += 1;
          break;
        }
    }

    return cur;
}

vector< pair<int, vector<vector<int>>> > split_option_finder::check_final_bw_div_in_table(vector< pair<int, vector<vector<int>>> > final_bw_div, vector<pair<int, int>> paths_bw){

    for (int i = 0; i < final_bw_div.size(); i++) {
        int path_id = final_bw_div[i].first;
        int path_bit_rate_index = reach_table_instance->get_bit_rate_index(paths_bw[i].second);
        int path_splits = final_bw_div[i].second[0].size();
//        cout << path_id << " " << path_bit_rate_index << " " << path_splits << endl;

        int size = final_bw_div[i].second.size();
        int it = 0;
        int ind = 0;
        while(it < size){
            int incr = 1;

            if(dynamicTable[path_id][path_bit_rate_index][path_splits-1][ind].size() == 0){
                incr = 0;
                size -= 1;
                final_bw_div[i].second.erase(final_bw_div[i].second.begin()+it);
//                cout << "caught\n";
            }

            ind += 1;
            it += incr;
        }

        if(final_bw_div[i].second.size() == 0){
//            cout << "returning\n";
            return {};
        }
    }

    return final_bw_div;
}

void split_option_finder::solveProtectedPaths(vector<vector<int>> paths, int index_of_path) {
    printPy("Path to be Solved:\n", debug); print2DVector(paths, debug);
    int size_of_path = paths.size();
    int number_of_paths = this->allVNPaths.paths.size();
    vector<int> bitRatesVector = this->all_bit_rates;
    printPy("bit rates Vec: ", debug); printVector(bitRatesVector, debug);
    vector<pair<int, vector<int>>> solutionVec = {};

    vector<vector<int>> all_divisions = intPermutationsHelper(bit_rate, reach_table_instance->get_all_bit_rates_vector(), size_of_path);
    printPy("all divisions\n", debug); print2DVector(all_divisions, debug);
    for (int all_division_it = 0; all_division_it < all_divisions.size(); all_division_it++) {
//      int min_slices_for_this_division = 100000;
//      solution solution_for_this_div = solution();
      //FOR EACH DIVISION
//      printPy("Division: ", debug); printVector(all_divisions[all_division_it], debug);

        vector<pair<int, int>> paths_bw = divide_and_merge(all_divisions[all_division_it], paths);
        sort(paths_bw.begin(), paths_bw.end(), [](pair<int, int> a, pair<int, int> b){
            return a.first < b.first;
        });
        for(int number_of_splits = paths_bw.size(); number_of_splits <= max_number_of_splits; number_of_splits++) {
        //FOR EACH POSSIBLE q <= Q
            vector<vector<int>> all_split_combinations = intPermutationsHelper(number_of_splits, {}, paths_bw.size());
            printPy("all split combinations\n", debug); print2DVector(all_split_combinations, debug);
            for (int split_comb_it = 0; split_comb_it < all_split_combinations.size(); split_comb_it++) {
                printPy("Split Comb: ", debug); printVector(all_split_combinations[split_comb_it], debug);
                vector< pair<int, vector<vector<int>>> > final_bw_div = this->calculateFinalBWDiv(paths_bw, all_split_combinations[split_comb_it], bitRatesVector);
                final_bw_div = this->check_final_bw_div_in_table(final_bw_div, paths_bw);
                if(final_bw_div.size() == 0){
                    printPy("Final BW division not found.\n", debug);
                    continue;
                }

                bool status = true;
                vector<int> curComb = {};
                vector<int> maxComb = {};
                int totalComb = 1;
                for (int i = 0; i < final_bw_div.size(); i++) {
                  totalComb *= final_bw_div[i].second.size();
                  curComb.push_back(0);
                  maxComb.push_back(final_bw_div[i].second.size()-1);
                }
                printPy("max comb: ", debug); printVector(maxComb, debug);
                for (int i = 0; i < totalComb; i++) {
//                    cout << index_of_path << " " << all_division_it << "/" << all_divisions.size() << " " << number_of_splits << " " << split_comb_it << "/" << all_split_combinations.size() << " " << i << "/" << totalComb << endl;
                    printPy("cur comb: ", debug); printVector(curComb, debug);
                    vector<pair<int, vector<int>>> selectedComb = {};
                    for (int j = 0; j < curComb.size(); j++) {
                        selectedComb.push_back(make_pair(final_bw_div[j].first, final_bw_div[j].second[curComb[j]]));
                        printPy("path: ", debug); printPy(final_bw_div[j].first, debug); printPy(" ", debug); printPy("bitrates: ", debug); printVector(final_bw_div[j].second[curComb[j]], debug);
                    }
                    solution newSolution = return_solution(selectedComb, this->min_slices);
                    if(newSolution.get_needed_slices() != -1){
                        this->optimalSolution = newSolution;
                        this->min_slices = newSolution.get_needed_slices();
                        this->vnSolutions[index_of_path].push_back(newSolution);
                    }
                    curComb = find_next_comb(curComb, maxComb);
                }
            }
        }
//        if(solution_for_this_div.get_needed_slices() != -1)
//            this->vnSolutions[index_of_path].push_back(solution_for_this_div);
    }
}

void split_option_finder::fill_dynamic_table(int K){
    vector<int>::iterator upper_bound_index = upper_bound(this->all_bit_rates.begin(), this->all_bit_rates.end(), this->bit_rate);
    vector<int> valid_bit_rate(this->all_bit_rates.begin(), upper_bound_index);
//    printVector(this->all_bit_rates);
    this->all_bit_rates = valid_bit_rate;
//    printPy(this->bit_rate);
//    printVector(this->all_bit_rates);

    //BASE CASES
    dynamicTable = {};
    for (int path_it = 0; path_it < K; path_it++) {
      dynamicTable.push_back({});
      int path_dist = this->allVNPaths.paths[path_it].dist;

        for (int bit_rate_it = 0; bit_rate_it < valid_bit_rate.size(); bit_rate_it++) {
          dynamicTable[path_it].push_back({});
          dynamicTable[path_it][bit_rate_it].push_back({});

          vector<int> tuple = this->reach_table_instance->get_best_tuple(path_dist, valid_bit_rate[bit_rate_it]);
          if(tuple.size() > 0){
//              cout <<"yes\n";
              dynamicTable[path_it][bit_rate_it][0].push_back({{tuple[0]}});
//              if(tuple[0] == 0)
//                  printVector(tuple);
          }else{
              dynamicTable[path_it][bit_rate_it][0].push_back({{}});
              dynamicTable[path_it][bit_rate_it][0][0].erase(dynamicTable[path_it][bit_rate_it][0][0].begin(), dynamicTable[path_it][bit_rate_it][0][0].end());
          }
        }
    }
    printVector(this->all_bit_rates, debug);
    for (int path_it = 0; path_it < K; path_it++) {
        for (int bit_rate_it = 0; bit_rate_it < valid_bit_rate.size(); bit_rate_it++) {
            printPy(path_it, debug); printPy(" ", debug); printPy(bit_rate_it, debug); printPy(" ", debug); printVector(dynamicTable[path_it][bit_rate_it][0][0], debug);
        }
    }

    //COMBINATION CASES
    for (int path_it = 0; path_it < K; path_it++) {
        int path_dist = this->allVNPaths.paths[path_it].dist;

        for (int bit_rate_it = 0; bit_rate_it < valid_bit_rate.size(); bit_rate_it++) {
            int temp_bit_rate = valid_bit_rate[bit_rate_it];

            for (int q = 1; q < this->max_number_of_splits; q++) {
                dynamicTable[path_it][bit_rate_it].push_back({});
                vector<vector<int>> bitrate_div = intPermutationsHelper(temp_bit_rate, valid_bit_rate, q+1);

                for (int div_it = 0; div_it < bitrate_div.size(); div_it++) {

                    vector<int> tuple_ids = {};
                    for (int i = 0; i < bitrate_div[div_it].size(); i++) {
                        int bit_index = findIndexOfNextInVector(valid_bit_rate, bitrate_div[div_it][i]);
                        if(dynamicTable[path_it][bit_index][0][0].size() == 1){
                            tuple_ids.push_back(dynamicTable[path_it][bit_index][0][0][0]);
                        }else{
                            tuple_ids = {};
                            tuple_ids.erase(tuple_ids.begin(), tuple_ids.end());
                            break;
                        }
                    }
                    dynamicTable[path_it][bit_rate_it][q].push_back(tuple_ids);
                }
            }
        }
    }

////
    printVector(this->all_bit_rates, debug);
    for (int path_it = 0; path_it < K; path_it++) {
        for (int bit_rate_it = 0; bit_rate_it < valid_bit_rate.size(); bit_rate_it++) {
            for (int q = 0; q < this->max_number_of_splits; q++) {

                vector<vector<int>> bitrate_div = intPermutationsHelper(valid_bit_rate[bit_rate_it], valid_bit_rate, q+1);
                for (int div_it = 0; div_it < bitrate_div.size(); div_it++) {
                    printPy(path_it, debug); printPy(" ", debug); printPy(bit_rate_it, debug); printPy(" ", debug); printPy(q, debug); printPy(" \t", debug);
                    printVector(bitrate_div[div_it], debug); printPy(" \t", debug); printVector(dynamicTable[path_it][bit_rate_it][q][div_it], debug);
                }

            }
        }
    }
    debug = false;

}

split_option_finder::split_option_finder(bool DBG, int K, double bsr_value, int Q, int bit_rate, vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> slices,
                protectedPathsOfPair_S paths, reach_table *reach_table_ins, vector<vector<int>> path_degrees, vector<int> next_demands, vector<vector<int>> adj_list){

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
//  this->convertor_instance = convertor_ins;
  this->reach_table_instance = reach_table_ins;
  this->allVNPaths = paths;
  printPy(paths.protectedPathsSet.size(), DBG); printPy(" <- # of paths\n", DBG);
  this->all_bit_rates = reach_table_ins->get_all_bit_rates_vector();
  printPy("all bit rates: ", DBG); printVector(this->all_bit_rates, DBG);
  printPy(paths.protectedPathsSet.size(), DBG); printPy(" <- # of paths\n", DBG);
  this->path_degrees = path_degrees;
  this->next_demands = next_demands;
  this->adj_list = adj_list;
  printPy("adj list:\n", DBG); print2DVector(adj_list, DBG);
  this->debug = DBG;
  vector<pair<int, vector<int>>> cur_sol = {};

  this->fill_dynamic_table(K);
  this->optimalSolution = solution();
  this->min_slices = 999999;
//  return;
/*  dynamic_table = {};
  for (int i = 0; i < K; i++) {
      dynamic_table.push_back({});
      for (int j = 0; j < this->all_bit_rates.size(); j++) {
          dynamic_table[i].push_back({-1});
      }
  }*/

  this->vnSolutions = {};
  for(int PPSet_it = 0; PPSet_it < paths.protectedPathsSet.size(); PPSet_it++){
      this->vnSolutions.push_back({});
      this->solveProtectedPaths(paths.protectedPathsSet[PPSet_it], PPSet_it);
      sort(vnSolutions[PPSet_it].begin(), vnSolutions[PPSet_it].end(), [](solution a, solution b){
          return a.get_needed_slices() < b.get_needed_slices();
      });
  }

/*  bool optimalSolutionSet = false;

  for(int i = 0; i < this->vnSolutions.size(); i++){
    if(this->vnSolutions[i].size() > 0){
        if(!optimalSolutionSet){
            this->optimalSolution = this->vnSolutions[i][0];
            optimalSolutionSet = true;
        } else if(this->vnSolutions[i][0].get_needed_slices() < this->optimalSolution.get_needed_slices()){
            this->optimalSolution = this->vnSolutions[i][0];
        }
    }
  }
*/
}
