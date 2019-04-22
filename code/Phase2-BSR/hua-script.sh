#!/bin/bash
clear

for bsrVal in 20 40 60 80 100
do
	mkdir $2/bsr$bsrVal
	for cases in 0 1 2
	do
		mkdir $2/bsr$bsrVal/case$cases
		for vnid in 0 1 2 3 4
		do
			commandname="./vne_heuristic"
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
