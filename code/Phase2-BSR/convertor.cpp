//
// Created by sepehr on 07/09/18.
//

#include "convertor.h"

vector<int> convertor::next_combination(vector<int> combination) {
    int remaining_sum = sum;
    vector<int> next_combination;

    for(int i = combination.size()-1; i >= 0; i--) {
        if(remaining_sum == combination[i]) {
            if(i == combination.size()-1)
                return vector<int>(0);
            next_combination[0]++;
            remaining_sum--;
            for(int j=i; j>=1; j--)
                next_combination.insert(next_combination.begin(), 0);
            next_combination.insert(next_combination.begin(), remaining_sum);
            break;
        }
        else {
            remaining_sum -= combination[i];
            next_combination.insert(next_combination.begin(), combination[i]);
        }
    }
    return next_combination;
}

int convertor::combination_to_int(vector<int> combination) {
    int base = sum + 1;
    int result = 0;
    for(int i=combination.size()-1; i>=0; i--) {
        result *= base;
        result += combination[i];
    }
    return result;
}

void convertor::initializer() {
    vector<int> combination;
    combination.push_back(sum);
    for(int i=1; i<n; i++)
        combination.push_back(0);
    int index = 0;
    do {
        int combination_value = combination_to_int(combination);

        combination_to_index[combination_value] = index;
        index_to_combination[index] = combination_value;
        index++;
        combination = next_combination(combination);
    }while(combination.size() != 0);
    number_of_combinations = index;
}

convertor::convertor(int number_of_paths, int number_of_splits) {
    this->n = number_of_paths + 1;
    this->sum = number_of_splits;
    initializer();
}

int convertor::get_number_of_combinations() {
    return number_of_combinations;
}

int convertor::get_combination_index(vector<int> combination) {
    int combination_value = combination_to_int(combination);
    return combination_to_index[combination_value];
}

vector<int> convertor::int_to_combination(int value) {
    vector<int> combination;
    int base = sum + 1;
    int temp = value;

    for(int i=0; i<n; i++) {
        int next_digit = temp % base;
        combination.push_back(next_digit);
        temp /= base;
    }

    return combination;
}

vector<vector<int>> convertor::get_parent_cases(int combination_value) {
    vector<vector<int>> parent_cases;
    int base = sum + 1;
    for(int path_id = 0; path_id < n - 1; path_id++) {
        int number_of_usage = combination_value % base;
        if(number_of_usage != 0) {
            vector<int> parent( n, 0);
            parent[path_id] = number_of_usage;
            parent[n - 1] = sum - number_of_usage;
            parent_cases.push_back(parent);
        }
        combination_value /= base;
        if(combination_value == 0)
            break;
    }
    return parent_cases;
}

void convertor::print_all_combinations_in_order() {
    for(int i=0; i<number_of_combinations; i++) {
        int combination_value = index_to_combination[i];
        vector<int> combination = int_to_combination(combination_value);
        cout<<i<<":\t";
        for(int j=0; j<combination.size(); j++)
            cout<<combination[j]<<"\t";
        cout<<endl;
    }
}

bool convertor::is_base_combination(vector<int> combination) {
    return get_number_of_distinct_members(combination) < 2;
}

int convertor::get_number_of_distinct_members(vector<int> combination) {
    int number_of_distinct_members = 0;
    for(int i = 0; i<n-1; i++)
        if(combination[i] > 0)
            number_of_distinct_members++;

    return number_of_distinct_members;
}

int convertor::get_combination_value_by_index(int combination_index) {
    return index_to_combination[combination_index];
}

vector<int> convertor::get_combination_path_ids(vector<int> combination) {
    vector<int> path_ids(0);
    for(int i=0; i<n-1; i++) {
        for(int j=0; j<combination[i]; j++)
            path_ids.push_back(i);
    }
    return path_ids;
}
