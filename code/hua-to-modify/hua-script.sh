#!/bin/bash
clear

# testcases-torun/varl2nvn-uniform-demands/20150729/case0/vnr/random/

for cases in 0 1 2
do
	for vnid in {0..4}
	do
		commandname="./vne_heuristic"
		pntopology=" --pn_topology_file=varl2nvn//data/case"$cases"//sn.txt"
		vntopology=" --vn_topology_file=varl2nvn//data/case"$cases"//vnr//vn"$vnid".txt"
		location=" --location_constraint_file=varl2nvn//data/case"$cases"//vnr//vnloc"$vnid".txt"
		finalcommand=$commandname$pntopology$vntopology$location
		echo $finalcommand
		$finalcommand | tee "varl2nvn//case"$cases"//vnr//newoutput"$vnid".txt"
	done
done
#fileoptimal="testoptimal.txt"
#fileheuristic="testheuristic.txt"

#read lineoptimal<
#"$fileoptimal"
#echo "$lineoptimal"

#read lineheuristic<"$fileheuristic"
#echo "$lineheuristic"

#echo "$lineheuristic / $lineoptimal - 1" | bc -l

# this works fine
# ./vneprotection/vne_protection --pn_topology_file=vneprotection/data/test1/test_p	n.topo --vn_topology_file=vneprotection/data/test1/test_vn.topo

# this does not work

