# WWPaf
To get the code type:

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
git clone https://github.com/NTrevisani/WWPaf.git WWPaf
cd WWPaf
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 This code runs on gridui.ifca.es with the following setup for root and PROOF:

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
source /cvmfs/cms.cern.ch/cmsset_default.sh

source /gpfs/csic_projects/cms/sw/ROOT/current/root/bin/thisroot.sh

export PYTHONPATH=$PYTHONPATH:/opt/glite/lib

export PAFPATH=/gpfs/csic_projects/cms/PROOF/paf/

export PATH=$PAFPATH/bin:$PATH
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 To run type: 

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
./runAllJets.sh "njets"

where "njets" = 0,1,2
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 ok, it's a nonsense, but we've not completely moved to the "all jets" analysis up to now

 root files are stored in rootFiles/"njets"jets/"channel"

 where "channel" indicates the decay channel (EE,EMu,MuE,SF,OF,All)

 the analysis is carried on in:

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
test.h

test.C
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 the main program is:

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
RunPROOF_test.C
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 For a test-run type:

 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
mkdir rootFiles/AllJet/EMu

root -l -b -q 'RunPROOF_test.C(19.365,"test","DataRun2012_Total",0,"EMu")'
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
