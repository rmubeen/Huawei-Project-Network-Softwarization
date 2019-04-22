#!/bin/bash
clear

# testcases-torun/varl2nvn-uniform-demands/20150729/case0/vnr/random/

for bsrVal in {20..100..20}
do
	mkdir $2/bsr$bsrVal
	for cases in 1
	do
		mkdir $2/bsr$bsrVal/case$cases
		for vnid in {0..19..1}
		do
			commandname="./vne_heuristic_flex_large"
			pntopology=" --pn_topology_file="$1"/case"$cases"/sn.txt"
			vntopology=" --vn_topology_file="$1"/case"$cases"/vnr/vn"$vnid".txt"
			location=" --location_constraint_file="$1"/case"$cases"/vnr/vnloc"$vnid".txt"
			output=" --output="$2"/bsr"$bsrVal"/case"$cases"/vn-n"$vnid"-"$bsrVal".txt"
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
