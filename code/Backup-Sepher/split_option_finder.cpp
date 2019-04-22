//
// Created by sepehr on 07/09/18.
//

#include "split_option_finder.h"
//#include "basic_functions.cpp"

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
    int needed_slices = 0;
    int number_of_tuples = best_tuples.size();

    for(int i = 0; i < number_of_tuples; i++)
        needed_slices += best_tuples[i][3];

    needed_slices *= number_of_hops[path_id];

    if(current_solution_obj != -1 && needed_slices >= current_solution_obj)
        return solution(); //return an invalid solution

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
        if(flag) {
            vector<int> solution_path(number_of_tuples, path_id);
            vector<int> solution_tuple_id;
            for(int l = 0; l < number_of_tuples; l++)
                solution_tuple_id.push_back(best_tuples[l][0]);

            return solution(solution_path, solution_tuple_id, vector<int>(begining_slices, begining_slices+number_of_tuples), needed_slices, path_degrees, reach_table_instance, next_demands);
        }
    }
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
        if(flag)
            return solution(path_ids, tuple_ids, vector<int>(begining_slices, begining_slices + number_of_tuples),
                            needed_slices, path_degrees, reach_table_instance, next_demands);
    }
    return solution();
}

void split_option_finder::solve_dp() {
    // base cases
    vector<int> bit_rates = reach_table_instance->get_all_bit_rates_vector();
//    cout << "\nbit rates: "; printVector(bit_rates); cout << endl;
    for(int bit_rate_index = 0; bit_rate_index < bit_rates.size() && bit_rates[bit_rate_index] <= bit_rate; bit_rate_index++) {
      // FOR EACH VALID BIT RATE
        for(int number_of_splits = 1; number_of_splits <= max_number_of_splits; number_of_splits++) {
          //FOR EACH VALID SPLIT
            vector<vector<int>> all_bit_rate_combinations = reach_table_instance->get_all_bit_rate_combination_with_specific_sum(bit_rate_index, number_of_splits);
            for(int p = 0; p < number_of_paths; p++) {
              //FOR EACH PATH
                vector<int> current_combination(0);
                for(int i = 0; i < number_of_paths+1; i++)
                    current_combination.push_back(0);
                current_combination[p] = number_of_splits;
                current_combination[number_of_paths] = max_number_of_splits - number_of_splits;

                int current_combination_index = convertor_instance->get_combination_index(current_combination);

                int current_solution_value = -1;

                for(int i = 0; i < all_bit_rate_combinations.size(); i++) {
                  // FOR ALL BIT RATE COMBINATIONS
                    vector<vector<int>> best_tuples(0);
                    bool possible = true;
                    for(int j = 0; j < number_of_splits; j++) {

                        vector<int> tuple = reach_table_instance->get_best_tuple(path_distances[p], all_bit_rate_combinations[i][j]); // optimization
                        if(tuple.size() == 0) {
                            possible = false;
                            break;
                        }

                        best_tuples.push_back(tuple);
                    }
                    if(possible) {
                        solution new_solution = number_of_slices_needed_base_cases(p, best_tuples, current_solution_value);
                        if(new_solution.get_needed_slices() != -1) {
                            dynamic_table[bit_rate_index][current_combination_index] = new_solution;
                            current_solution_value = new_solution.get_needed_slices();
                        }
                    }
                }
            }
        }
    }

    //non-base cases

    int number_of_combinations = convertor_instance->get_number_of_combinations();
    vector<vector<int>> **all_sum_combinations = reach_table_instance->get_all_sum_combinations();

  //  printPy("all sum combinations\n"); //printPy(all_sum_combinations);
//    reach_table_instance->print_sum_combinations();

    for(int i = 0; i < number_of_combinations; i++) {
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

        vector<int> this_comb_path_ids = convertor_instance->get_combination_path_ids(combination);

        for(int bit_rate_index = 0; bit_rate_index < bit_rates.size() && bit_rates[bit_rate_index] <= bit_rate; bit_rate_index++) {
            int current_solution_value = dynamic_table[bit_rate_index][current_combination_index].get_needed_slices();

            for(int sum_comb_index = 0; sum_comb_index < all_sum_combinations[bit_rate_index][number_of_distincts_path].size(); sum_comb_index++) {
                vector<int> this_comb_tuple_ids(0);
                bool flag = true;
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
                if(flag && (current_solution_value == -1 || current_solution_value > needed_slices_for_this_sum_combination)) {
                    solution solution_using_sum_combiantion = find_solution(this_comb_path_ids, this_comb_tuple_ids);
                    if(solution_using_sum_combiantion.get_needed_slices() != -1) {
                        dynamic_table[bit_rate_index][current_combination_index] = solution_using_sum_combiantion;
                        current_solution_value = solution_using_sum_combiantion.get_needed_slices();
                    }
                }
            }
        }
    }

    // going through all the combination and find the valid solutions for the vlink
    for(int i = 0; i < number_of_combinations; i++)
        if(dynamic_table[reach_table_instance->get_bit_rate_index(bit_rate)][i].get_needed_slices() != -1) {
            solution_array.push_back(dynamic_table[reach_table_instance->get_bit_rate_index(bit_rate)][i]);
        }

    sort(solution_array.begin(), solution_array.begin()+solution_array.size());
}

split_option_finder::split_option_finder(int bit_rate, int q, vector<bitset<NUMBER_OF_PHYSICAL_SLICES>> slices, vector<int> distances, vector<int> hops, vector<vector<int>> adjacency_list, convertor *convertor_ins, reach_table *reach_table_ins, vector<vector<int>> path_degrees, vector<int> next_demands) {
    this->src = src;
    this->dst = dst;
    this->bit_rate = bit_rate;
    number_of_paths = distances.size();
    max_number_of_splits = q;

    this->path_degrees = path_degrees;
    this->next_demands = next_demands;

    path_slices = slices;
    number_of_hops = hops;
    path_distances = distances;
    adj_list = adjacency_list;


    convertor_instance = convertor_ins;
    reach_table_instance = reach_table_ins;

    int number_of_bit_rates = reach_table_instance->get_number_of_bit_rates();
    int number_of_combinations = convertor_instance->get_number_of_combinations();
    dynamic_table = new solution*[number_of_bit_rates];
    for(int i=0; i<number_of_bit_rates; i++)
        dynamic_table[i] = new solution[number_of_combinations];

    solve_dp();
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

void split_option_finder::print_solutions() {
    for(int i=0; i<solution_array.size(); i++) {
        solution x = solution_array[i];
        int needed_slices = x.get_needed_slices();
        vector<int> path_ids = x.get_paths();
        vector<int> tuple_ids = x.get_tuples();
        vector<int> starting_slice_for_each_tuple = x.get_starting_slices();

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
