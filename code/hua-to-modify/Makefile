
FILES = bsr_main.cpp io.h util.cpp reach_table.cpp convertor.cpp split_option_finder.cpp reading_utilities.cpp solution.h problem_spec.cpp

all:
	g++ -O2 -std=c++0x $(LIB_PATHS) $(INCLUDE_PATHS) $(FILES) $(LIBS) -o vne_heuristic

dbg:
	g++ -DDBG -g -std=c++0x $(LIB_PATHS) $(INCLUDE_PATHS) $(FILES) $(LIBS) -o vne_heuristic

debug:
	g++ -g -std=c++0x $(LIB_PATHS) $(INCLUDE_PATHS) $(FILES) $(LIBS) -o vne_heuristic
