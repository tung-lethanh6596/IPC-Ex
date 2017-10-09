#!/bin/sh
echo $1 |sed 's/;/\n/g' > tmp.txt
testcases=""
testfiles=""
oldtestfile=""
while read -r test
do
    testcase=`echo $test | cut -d'/' -f4`
    testfile=`echo $test | cut -d'/' -f1,2,3`
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
