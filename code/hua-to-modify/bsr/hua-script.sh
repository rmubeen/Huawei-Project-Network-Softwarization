#!/bin/bash
clear

# testcases-torun/varl2nvn-uniform-demands/20150729/case0/vnr/random/

for cases in 0
do
	mkdir output/case$cases
	for bsrVal in 20
	do
		mkdir output/case$cases/bsr$bsrVal
		for vnid in 0 1 2 3 4
		do
			commandname="./vne_heuristic"
			pntopology=" --pn_topology_file=input/vn-n4/case"$cases"/sn.txt"
			vntopology=" --vn_topology_file=input/vn-n4/case"$cases"/vnr/vn"$vnid".txt"
			location=" --location_constraint_file=input/vn-n4/case"$cases"/vnr/vnloc"$vnid".txt"
			output=" --output=output/case"$cases"/bsr"$bsrVal"/vn-n"$vnid"-"$bsrVal".txt"
			bsr=" --bsr="$bsrVal
			finalcommand=$commandname$pntopology$vntopology$location$output$bsr
			echo $finalcommand
			$finalcommand
		done
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

