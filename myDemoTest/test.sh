#!/bin/bash
res=`echo $tests | grep "testcase" -o | wc -l`
for (( i=2; $i<=($res + 1); i++))
do
	testfile=`echo $tests | cut -d'{' -f$i | cut -d',' -f5 | cut -d':' -f2 | sed 's/"//g' | sed 's/]//g' | sed 's/}//g'`
	testcase=`echo $tests | cut -d'{' -f$i | cut -d',' -f4 | cut -d':' -f2 | sed 's/"//g' | sed 's/]//g' | sed 's/}//g'`	
	echo "testcase: $testcase, testfile: $testfile"
done
