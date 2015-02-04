if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./run.sh NJETS"
     echo "  "
     exit -1
fi


LUMINOSITY=19.468

TEST="test"

NJETS=$1

CHANNELS="EE MuE EMu MuMu "
#EE MuE EMu MuMu "

SAMPLES="          \
DataRun2012_Total  \
ggWWto2L           \
WWTo2L2Nu          \
WWTo2L2Nu_pow      \
WWTo2L2Nu_mcnlo    \
GamGamWW           \
WZ                 \
ZZ                 \
TTbar              \
TW                 \
DY                 \
WgammaStar         \
WgammaNoStar       \
DYtautau           \
Zgamma             \
HWW125             \
VVV                \
WJets              \
WJetsFakes_Total   \
"

#rm -rf rootfiles/${NJETS}jet

mkdir rootFiles

# Loop
for CHANNEL in $CHANNELS; do

    for SAMPLE in $SAMPLES; do 
	
	mkdir rootFiles/${NJETS}jets/
	mkdir rootFiles/${NJETS}jets/${CHANNEL}	
	root -l -b -q "RunPROOF_test.C($LUMINOSITY,\"$TEST\",\"$SAMPLE\","$NJETS",\"$CHANNEL\")"
	mv ${SAMPLE}.root rootFiles/${NJETS}jets/${CHANNEL}
  
    done

    OUTPATH=rootfiles/${NJETS}jets/${CHANNEL}

#    Merge some MC files
    hadd ${OUTPATH}/WW.root     ${OUTPATH}/ggWWto2L.root     ${OUTPATH}/WWTo2L2Nu.root 
    hadd ${OUTPATH}/WW_pow.root     ${OUTPATH}/ggWWto2L.root     ${OUTPATH}/WWTo2L2Nu_pow.root 
    hadd ${OUTPATH}/WW_mc.root     ${OUTPATH}/ggWWto2L.root     ${OUTPATH}/WWTo2L2Nu_mcnlo.root
    hadd ${OUTPATH}/Wgamma.root ${OUTPATH}/WgammaNoStar.root ${OUTPATH}/WgammaStar.root ${OUTPATH}/GamGamWW.root 

done


