//
// Created by sepehr on 05/09/18.
//

#include "reading_utilities.h"

string reading_utilities::get_nth_item(string s, int n, string delimiter) {
    int index = 0;
    while(index < n) {
        if(s.find(delimiter) == string::npos)
            return "";
        s = s.substr(s.find(delimiter) + delimiter.length(), s.size());
        index++;
    }
    return s.substr(0, s.find(delimiter));
}