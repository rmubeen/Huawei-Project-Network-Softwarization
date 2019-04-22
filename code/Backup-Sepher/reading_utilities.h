//
// Created by sepehr on 05/09/18.
//

#ifndef HUA_TO_MODIFY_READING_UTILITIES_H
#define HUA_TO_MODIFY_READING_UTILITIES_H

#include <string>
#include <stdexcept>
#include <iostream>
using namespace std;

class reading_utilities {
public:
    static string get_nth_item(string s, int n, string delimiter);
};


#endif //HUA_TO_MODIFY_READING_UTILITIES_H
