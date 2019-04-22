//
// Created by sepehr on 05/09/18.
//

#ifndef HUA_TO_MODIFY_READING_UTILITIES_H
#define HUA_TO_MODIFY_READING_UTILITIES_H

#include <string>
#include <stdexcept>
#include <iostream>
using namespace std;

/*
 * This class assists in reading input data.
 */
class reading_utilities {
public:
  /*
   * It takes a string, an index and a delimeter as an input.
   * And, returns substring on nth index in the given string.
   */
    static string get_nth_item(string s, int n, string delimiter);
};


#endif //HUA_TO_MODIFY_READING_UTILITIES_H
