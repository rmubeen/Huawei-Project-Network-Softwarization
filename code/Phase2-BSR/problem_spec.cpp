//
// Created by sepehr on 10/08/18.
//

#include "problem_spec.h"
#define sn_file "sn.txt"

problem_spec::problem_spec(string path_file_path, string vn_path, string vn_loc_path) {
    fin = ifstream(path_file_path);
    read_file(path);
    fin.close();

    fin = ifstream(vn_path);
    read_file(vn);
    fin.close();

    fin = ifstream(vn_loc_path);
    read_file(vn_loc);
    fin.close();

    create_adjacency_list();
    find_best_order();
    find_path_degrees_to_next_vlinks();
}

void problem_spec::create_adjacency_list() {
    adjacency_list = vector<vector<int>> (shortest_paths.size(), vector<int> (0));
    int s_src = -1;
    int s_dst = -1;
    int vlink_indexes = -1;

    int first_hop_index = 8;

    for(int i = 0; i < shortest_paths.size(); i++) {

        int u = shortest_paths[i][1];
        int v = shortest_paths[i][2];

        if(u != s_src || v != s_dst) {
            vlink_indexes++;
            if(vlink_indexes != 0)
                vlink_shortest_paths_indexes[vlink_indexes-1].push_back(i);

            vlink_shortest_paths_indexes.push_back(vector<int> ({u, v, i}));

            s_src = u;
            s_dst = v;
        }

        if(i == shortest_paths.size() - 1)
            vlink_shortest_paths_indexes[vlink_indexes].push_back(i + 1);



        int number_of_links_1 = shortest_paths[i][6];

        for(int j = i+1; j < shortest_paths.size(); j++) {
            int number_of_links_2 = shortest_paths[j][6];

            bool is_adjacent = false;

            for(int k = 0; k < number_of_links_1; k++) {
                if(is_adjacent)
                    break;
                int u1 = shortest_paths[i][first_hop_index + k];
                int v1 = shortest_paths[i][first_hop_index + k + 1];

                for(int l = 0; l < number_of_links_2; l++) {
                    int u2 = shortest_paths[j][first_hop_index + l];
                    int v2 = shortest_paths[j][first_hop_index + l + 1];

                    if((u1 == u2 && v1 == v2) || (u1 == v2 && v1 == u2)) {
                        adjacency_list[i].push_back(j);
                        adjacency_list[j].push_back(i);
                        is_adjacent = true;
                        break;
                    }
                }
            }
        }
    }

//    for(int i = 0; i < adjacency_list.size(); i++) {
//        cout<<"index: "<<i<<" size: "<<adjacency_list[i].size()<<" ";
//        for(int j = 0; j < adjacency_list[i].size(); j++) {
//            cout<<adjacency_list[i][j]<<" ";
//        }
//        cout<<endl;
//    }
//
//    for(int i = 0; i < vlink_shortest_paths_indexes.size(); i++) {
//        for(int j = 0; j < vlink_shortest_paths_indexes[i].size(); j++)
//            cout<<vlink_shortest_paths_indexes[i][j]<<" ";
//        cout<<endl;
//    }
}

void problem_spec::read_file(file_type input_file_type) {
    string line;
    int line_number = 0;
    while(getline(fin, line)) {
        switch(input_file_type) {
            case path: {
                vector<int> current_line;

                int v_src = stoi(reading_utilities::get_nth_item(line, 0, " "));
                int v_dst = stoi(reading_utilities::get_nth_item(line, 1, " "));
                int s_src = stoi(reading_utilities::get_nth_item(line, 2, " "));
                int s_dst = stoi(reading_utilities::get_nth_item(line, 3, " "));
                int index = stoi(reading_utilities::get_nth_item(line, 4, " "));
                int hops = stoi(reading_utilities::get_nth_item(line, 5, " "));
                int distance = int(stof(reading_utilities::get_nth_item(line, 6, " ")));

                int arr[] = {line_number, v_src, v_dst, s_src, s_dst, index, hops, distance};
                current_line.assign(arr, arr + sizeof(arr) / sizeof(arr[0]));

                int first_hop = 7;

                for (int i = 0; i < hops; i++) {
                    if (i == 0) {
                        int u = stoi(reading_utilities::get_nth_item(line, first_hop + 2 * i, " "));
                        current_line.push_back(u);
                    }

                    int v = stoi(reading_utilities::get_nth_item(line, first_hop + 2 * i + 1, " "));
                    current_line.push_back(v);
                }

//                for (int i = 0; i < current_line.size(); i++) {
//                    cout << current_line[i] << " ";
//                }
//                cout << endl;
                shortest_paths.push_back(current_line);

            }
            break;
            case vn: {
                int id = stoi(reading_utilities::get_nth_item(line, 0, ","));
                int src = stoi(reading_utilities::get_nth_item(line, 1, ","));
                int dst = stoi(reading_utilities::get_nth_item(line, 2, ","));
                int demand = stoi(reading_utilities::get_nth_item(line, 5, ","));
                vlinks.push_back(vector<int>({id, src, dst, demand}));

//                cout << "id: " << id << " src: " << src << " dst: " << dst << " demand: " << demand << endl;
            }
            break;
            case vn_loc: {
                vector<int> current_line_constraint;
                int v_node = stoi(reading_utilities::get_nth_item(line, 0, ","));
                current_line_constraint.push_back(v_node);
                int x = 1;
                while (true) {
                    string s = reading_utilities::get_nth_item(line, x, ",");
                    if (s != "") {
//                        cout<<s<<endl;
                        int s_node = stoi(s);
                        current_line_constraint.push_back(s_node);
                        x++;
                    }
                    else
                        break;
                }
//                for (int i = 0; i < current_line_constraint.size(); i++)
//                    cout << current_line_constraint[i] << " ";
//                cout << endl;
                loc_constraint.push_back(current_line_constraint);
            }
        }
        line_number++;
    }
}

void problem_spec::find_best_order() {

    vlink_order = vector<int> (0);

    vector<vector<int>> current_adj_list(adjacency_list);
    bool *mark = new bool[vlinks.size()];

    for(int i = 0; i < vlinks.size(); i++) {
        int first_path_index = vlink_shortest_paths_indexes[i][2];
        int last_path_index = vlink_shortest_paths_indexes[i][3];

        for(int j = first_path_index; j < last_path_index; j++) {

            vector<int>::iterator beginning_it = lower_bound(current_adj_list[j].begin(), current_adj_list[j].end(),
                                                            first_path_index);
            vector<int>::iterator ending_it = lower_bound(current_adj_list[j].begin(), current_adj_list[j].end(),
                                                          last_path_index);

            current_adj_list[j].erase(beginning_it, ending_it);
        }
    }

    for(int i = 0; i < vlinks.size(); i++)
        mark[i] = false;

    for(int i = 0; i < vlinks.size(); i++) {
        int best_index = -1;
        int best_degree = -1;


        for(int j = 0; j < vlinks.size(); j++) {
            if(mark[j] == true)
                continue;

            int first_path_index = vlink_shortest_paths_indexes[j][2];
            int last_path_index = vlink_shortest_paths_indexes[j][3];

            int current_vlink_weight = 0;
            for(int k = first_path_index; k < last_path_index; k++)
                current_vlink_weight += current_adj_list[k].size();

//            cout<<"index: "<<j<<" degree: "<<current_vlink_weight<<endl;

            if(best_index == -1 || (current_vlink_weight < best_degree || (current_vlink_weight == best_degree &&
                                                                                vlinks[j][3] > vlinks[best_index][3]))) {
                best_degree = current_vlink_weight;
                best_index = j;
            }
        }

        mark[best_index] = true;
        vlink_order.push_back(best_index);

        int best_node_first_path_index = vlink_shortest_paths_indexes[best_index][2];
        int best_node_last_path_index = vlink_shortest_paths_indexes[best_index][3];

//        cout<<"best index: "<<best_index<<" degree: "<<best_degree<<" first edge: "<<
//            best_node_first_path_index<<" last edge: "<<best_node_last_path_index<<endl;

        for(int j = 0; j < vlinks.size(); j++) {
            if(mark[j] == true)
                continue;

            int first_path_index = vlink_shortest_paths_indexes[j][2];
            int last_path_index = vlink_shortest_paths_indexes[j][3];

            for(int k = first_path_index; k < last_path_index; k++) {
                vector<int>::iterator beginning_it = lower_bound(current_adj_list[k].begin(), current_adj_list[k].end(),
                                                                best_node_first_path_index);
                vector<int>::iterator ending_it = lower_bound(current_adj_list[k].begin(), current_adj_list[k].end(),
                                                              best_node_last_path_index);
                current_adj_list[k].erase(beginning_it, ending_it);
            }
        }
    }

    reverse(vlink_order.begin(), vlink_order.end());
//    cout<<"best order: ";
//    for(int i = 0; i < vlink_order.size(); i++)
//        cout<<vlink_order[i]<<" ";
//    cout<<endl;
}


void problem_spec::find_path_degrees_to_next_vlinks() {
    path_degree_to_next_vlinks = vector<vector<int>> (shortest_paths.size(), vector<int> (0));

    for(int i = 0; i < vlink_order.size(); i++) {

        int node_index = vlink_order[i];

        int first_path = vlink_shortest_paths_indexes[node_index][2];
        int last_path = vlink_shortest_paths_indexes[node_index][3];

        for(int l = first_path; l < last_path; l++) {
            vector<int> current_node_adj_list(adjacency_list[l]);
//            cout<<"{ ";
            for(int j = i + 1; j < vlink_order.size(); j++) {
                int next_node_index = vlink_order[j];
                int first_path_index = vlink_shortest_paths_indexes[next_node_index][2];
                int last_path_index = vlink_shortest_paths_indexes[next_node_index][3];

                vector<int>::iterator b_it = lower_bound(current_node_adj_list.begin(), current_node_adj_list.end(), first_path_index);
                vector<int>::iterator l_it = lower_bound(current_node_adj_list.begin(), current_node_adj_list.end(), last_path_index);
                int deg = distance(b_it, l_it);
                path_degree_to_next_vlinks[l].push_back(deg);
//                cout<<deg<<", ";
            }
//            cout<<"} ";
        }
//        cout<<endl;
    }


//    for(int i = 0; i < vlink_order.size(); i++) {
//        int node_index = vlink_order[i];
//
//        int first_path = vlink_shortest_paths_indexes[node_index][2];
//        int last_path = vlink_shortest_paths_indexes[node_index][3];
//
//        for(int j = first_path; j < last_path; j++) {
//            cout<<j<<": ";
//            for(int k = 0; k < path_degree_to_next_vlinks[j].size(); k++) {
//                cout<<path_degree_to_next_vlinks[j][k]<<" ";
//            }
//            cout<<endl;
//        }
//        cout<<endl;
//    }
}
