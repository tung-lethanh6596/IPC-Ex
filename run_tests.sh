#!/bin/sh
echo $1 |sed 's/;/\n/g' > tmp.txt
testcases=""
testfiles=""
oldtestfile=""
while read -r test
do
    cnt=`echo ${test::-1} | sed 's/\//\n/g' | wc -l`
    cnt=$(($cnt - 1))
    before=$(($cnt - 1))
    testcase=`echo $test | cut -d'/' -f$cnt`
    testfile=`echo $test | cut -d'/' -f1-$before`
    testcases="$testcases -t \"$testcase\""
    if [ "$testfile" != "$oldtestfile" ]
    then
    	testfiles="$testfiles $testfile"
    fi
    oldtestfile=$testfile
done < tmp.txt
script="robot $testcases $testfiles"
echo $script
rm -f tmp.txt
