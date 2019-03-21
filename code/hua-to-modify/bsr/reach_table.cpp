//
// Created by sepehr on 07/09/18.
//

#include "reach_table.h"

void reach_table::check() {
    for(int i = 0; i < all_bit_rates.size(); i++) {
        for(int j = 1; j <= max_number_of_splits; j++) {
            cout<<"combinations["<<all_bit_rates[i]<<"]["<<j<<"]: {";
            for(int k = 0; k < base_combinations[i][j].size(); k++) {
                for(int l = 0; l < j; l++) {
                    if(l == 0)
                        cout<<"{";
                    cout<<base_combinations[i][j][k][l];
                    if(l == j-1)
                        cout<<"}";
                    else
                        cout<<", ";
                }
                if(k!=base_combinations[i][j].size()-1)
                    cout<<", ";
            }
            cout<<"}\n";
        }
    }
}

void reach_table::print_sum_combinations() {
    for(int q = 1; q <= number_of_paths; q++) {
        for(int b = 0; b < all_bit_rates.size(); b++) {
            cout<<"sum_combinations["<<all_bit_rates[b]<<"]["<<q<<"]: {";
            for(int k = 0; k < sum_combinations[b][q].size(); k++) {
                for (int l = 0; l < q; l++) {
                    if (l == 0)
                        cout << "{";
                    cout << sum_combinations[b][q][k][l];
                    if (l == q - 1)
                        cout << "}";
                    else
                        cout << ", ";
                }
                if (k != sum_combinations[b][q].size() - 1)
                    cout << ", ";
            }
            cout<<"}\n";
        }
    }
}

void reach_table::print_base_combinations() {
    for(int q = 1; q <= number_of_paths; q++) {
        for(int b = 0; b < all_bit_rates.size(); b++) {
            cout<<"base_combinations["<<all_bit_rates[b]<<"]["<<q<<"]: {";
            for(int k = 0; k < base_combinations[b][q].size(); k++) {
                for (int l = 0; l < q; l++) {
                    if (l == 0)
                        cout << "{";
                    cout << base_combinations[b][q][k][l];
                    if (l == q - 1)
                        cout << "}";
                    else
                        cout << ", ";
                }
                if (k != base_combinations[b][q].size() - 1)
                    cout << ", ";
            }
            cout<<"}\n";
        }
    }
}

void reach_table::generate_all_sum_combinations() {
    for(int b = 0; b < all_bit_rates.size(); b++) {
        sum_combinations[b][1].push_back(vector<int> ({all_bit_rates[b]}));
    }

    for(int b = 0; b < all_bit_rates.size(); b++) {
        for(int q = 2; q <= number_of_paths; q++) {
            for(int d = 0; d < b; d++) {
                if(all_bit_rates[b] - all_bit_rates[d] < all_bit_rates[0])
                    continue;
                int remaining_index = bit_rate_index[all_bit_rates[b] - all_bit_rates[d]];
                for(int i = 0; i < sum_combinations[remaining_index][q - 1].size(); i++) {
                    vector<int> parent_combination = sum_combinations[remaining_index][q - 1][i];
                    parent_combination.push_back(all_bit_rates[d]);
                    sum_combinations[b][q].push_back(parent_combination);
                }
            }
        }
    }
}

void reach_table::generate_all_base_combinations() {
    for(int b = 0; b < all_bit_rates.size(); b++) {
        if(tuple_bit_rate_to_first_index.find(all_bit_rates[b]) == tuple_bit_rate_to_first_index.end())
            continue;

        for(int i = 0; i < all_bit_rates.size(); i++) {
            for (int j=1; j <= max_number_of_splits; j++) {
                int remaining_data = all_bit_rates[i] - all_bit_rates[b];
                int remaining_splits = j - 1;
                if(remaining_splits == 0) {
                    if(remaining_data == 0) {
                        base_combinations[i][j].push_back(vector<int> ({all_bit_rates[b]}));
                    }
                }
                else if(remaining_data > 0 && bit_rate_index.find(remaining_data) != bit_rate_index.end()){
                    int parent_index = bit_rate_index[remaining_data];
                    for(int k = 0; k < base_combinations[parent_index][remaining_splits].size(); k++) {
                        vector<int> par_combination = base_combinations[parent_index][remaining_splits][k];
                        par_combination.push_back(all_bit_rates[b]);
                        base_combinations[i][j].push_back(par_combination);
                    }
                }
            }
        }
    }
}

void reach_table::print_all_permutations() {
    for(int q = 1; q <= max_number_of_splits; q++) {
        cout<<"permutations["<<q<<"]: {";
        for(int i = 0; i < permutations[q].size(); i++) {
            cout<<"{";
            for(int j = 0; j < q; j++) {
                cout<<permutations[q][i][j];
                if(j != q-1)
                    cout<<", ";
            }
            cout<<"}";
            if(i != permutations[q].size() - 1)
                cout<<", ";
        }
        cout<<"}\n";
    }
}

void reach_table::generate_all_permutations() {
    permutations[1].push_back(vector<int> ({0}));
    for(int q = 2; q <= max_number_of_splits; q++) {
        for(int i = 0; i < permutations[q-1].size(); i++) {
            vector<int> p = permutations[q-1][i];
            for(int j = q-1; j >= 0; j--) {
                p.insert(p.begin() + j, q - 1);
                permutations[q].push_back(p);
                p.erase(p.begin() + j);
            }
        }
    }
}


reach_table::reach_table(string file_path, int max_number_of_splits, int number_of_paths, int max_possible_data_rate, int smallest_bit_rate, int bit_rate_step) {
    this->max_number_of_splits = max_number_of_splits;
    this->number_of_paths = number_of_paths;
    ifstream fin(file_path);
    string line;
    int last_bit_rate = 0;
    while(getline(fin, line)) {
        int id = stoi(reading_utilities::get_nth_item(line, 1, ","));
//        cout<<id<<endl;
        string bit_rate_string = reading_utilities::get_nth_item(line, 2, ",");
        if(bit_rate_string.size() != 0) {
            last_bit_rate = stoi(bit_rate_string);
            tuple_bit_rate_to_first_index[last_bit_rate] = tuples.size();
            all_bit_rates.push_back(last_bit_rate);
        }
        int bit_rate = last_bit_rate;

        int needed_bandwidth = stoi(reading_utilities::get_nth_item(line, 8, ","));
        int needed_slices = (int)ceil((double)needed_bandwidth / slice_size);
        int distance = stoi(reading_utilities::get_nth_item(line, 9, ","));

        tuples.push_back(vector<int> ({id, bit_rate, distance, needed_slices}));
    }

    int all_bit_rates_index = 0;
    for(int data_rate = smallest_bit_rate; data_rate<=max_possible_data_rate; data_rate+=bit_rate_step) {
        while(data_rate > all_bit_rates[all_bit_rates_index] && all_bit_rates_index != all_bit_rates.size())
            all_bit_rates_index++;
        if(all_bit_rates_index == all_bit_rates.size())
            all_bit_rates.push_back(data_rate);
        else if(data_rate != all_bit_rates[all_bit_rates_index])
            all_bit_rates.insert(all_bit_rates.begin()+all_bit_rates_index, data_rate);
    }

    for(int i=0; i<all_bit_rates.size(); i++)
        bit_rate_index[all_bit_rates[i]] = i;

    base_combinations = new vector<vector<int>> *[all_bit_rates.size()];
    for(int i=0; i<all_bit_rates.size(); i++)
        base_combinations[i] = new vector<vector<int>>[this->max_number_of_splits + 1];

    sum_combinations = new vector<vector<int>> *[all_bit_rates.size()];
    for(int i=0; i<all_bit_rates.size(); i++)
        sum_combinations[i] = new vector<vector<int>>[this->number_of_paths + 1];

    permutations = new vector<vector<int>> [this->max_number_of_splits + 1];

    generate_all_base_combinations();
    generate_all_sum_combinations();
    generate_all_permutations();
}

vector<int> reach_table::get_all_bit_rates_vector() {
    return all_bit_rates;
}

int reach_table::get_tuple_needed_slices(int reach_table_index) {
    return tuples[reach_table_index - 1][3];
}

vector<vector<int>>** reach_table::get_all_sum_combinations() {
    return sum_combinations;
}

vector<vector<int>> reach_table::get_all_permutations(int n) {
    return permutations[n];
}

vector<vector<int>> reach_table::get_all_bit_rate_combination_with_specific_sum(int sum_index, int number_of_splits) {
    return base_combinations[sum_index][number_of_splits];
}

int reach_table::get_number_of_bit_rates() {
    return all_bit_rates.size();
}

int reach_table::get_bit_rate_index(int bit_rate) {
    return bit_rate_index[bit_rate];
}

vector<int> reach_table::get_best_tuple(int distance, int data_rate) {
    int min_slices_needed = 10000;
    map<int, int>::iterator it = tuple_bit_rate_to_first_index.find(data_rate);
    if(it == tuple_bit_rate_to_first_index.end())
        return vector<int> (0);

    int index = it->second;
    int best_index = -1;

    while(index < tuples.size() && tuples[index][1] == data_rate) {
        if(tuples[index][2] >= distance && min_slices_needed >= tuples[index][3]) {
            best_index = index;
            min_slices_needed = tuples[index][3];
        }
        index++;
    }

    if(best_index == -1)
        return vector<int> (0);
    return tuples[best_index];
}
