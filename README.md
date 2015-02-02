# WWPaf

WARNING: VERY EARLY VERSION!!

To run type: 

./run.sh 0

./run.sh 1

./run.sh 2

root files are stored in rootFiles/

the analysis is carried on in:

test.h

test.C

the main program is:

RunPROOF_test.C

For a test-run type:

mkdir rootFiles/0jets/EE

root -l -b -q 'RunPROOF_test.C(19.468,"test","DataRun2012_Total",0,"EE")'
