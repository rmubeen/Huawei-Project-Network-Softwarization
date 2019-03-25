#ifndef IO_H_
#define IO_H_

#include "util.h"

#include <map>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

//#define slice_width 50
#define slice_width 12.5


std::unique_ptr<std::map<std::string, std::string> > ParseArgs(int argc,
                                                               char *argv[]) {
    std::unique_ptr<std::map<std::string, std::string> > arg_map(
            new std::map<std::string, std::string>());
    for (int i = 1; i < argc; ++i) {
        char *key = strtok(argv[i], "=");
        char *value = strtok(NULL, "=");
        DEBUG(" [%s] => [%s]\n", key, value);
        arg_map->insert(std::make_pair(key, value));
    }
    return std::move(arg_map);
}



#endif // IO_H_
