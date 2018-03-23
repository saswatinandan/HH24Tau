#!/bin/bash
#
echo "input parameters: cluster, process, run path, out path, sample name" $1 $2
CLUSTER=$1
PROCESS=$2

#########For running locally uncommnet this line
#CLUSTER=""
#PROCESS=5
#RUNPATH="/uscms_data/d3/abdollah/Analysis/LQ2016/CMSSW_8_0_11/src/Skim_ggNtuple"
#OUTPATH=""


echo ""
echo "CMSSW on Condor"
echo ""

START_TIME=`/bin/date`
echo "started at $START_TIME"

echo ""
echo "parameter set:"
echo "CLUSTER: $CLUSTER"
echo "PROCESS: $PROCESS"

source /cvmfs/cms.cern.ch/cmsset_default.sh
echo ${_CONDOR_SCRATCH_DIR}
cd ${_CONDOR_SCRATCH_DIR}
echo $PWD , "for job running" 

setenv SCRAM_ARCH slc6_amd64_gcc530 

xrdcp -s root://cmseos.fnal.gov//store/user/snandan/CMSSW8026.tgz .
tar -xf CMSSW8026.tgz
rm CMSSW8026.tgz
cd CMSSW_8_0_26_patch1/src/
scramv1 b ProjectRename
cd ggAnalysis/ggNtuplizer/test/Skim_test
eval `scramv1 runtime -csh` # cmsenv is an alias not on the workers 
echo $PWD , "for cmsenv"


###### Copy the neccessary files for running the Skim


#########  Smaple/Job splitting
SplitingNumber=10
DataSetArray=($(cat l.txt)) # array of the input datadets
echo $DataSetArray
echo '*************' ${DataSetArray[$PROCESS / $SplitingNumber]}
DataSetName=${DataSetArray[$PROCESS / $SplitingNumber]}
echo 'DataSetName= ' $DataSetName
rootNumber=$(($PROCESS % $SplitingNumber))
echo 'root#= =' $rootNumber
#DataSetName=DataSetName_.replace("/store/user/abdollah/Moriond17/","")

########### complie the Skimmer
make
########### loop over all root file in a dataset directory
xrdfs root://cmseos.fnal.gov ls "/eos/uscms/"$DataSetName | grep $rootNumber.root | while read FullDataSetName

############  Here is where the Skimmer is running     ############
do
 file=`echo $FullDataSetName`
 ShortName=${file##*officialHHMc}  # This removes all the string before Moriond17 (including Moriond17)
# ShortName=${file##*diHiggs}
 echo "Here is the short Name   ------>" $ShortName
 ./Skimmer  $ShortName 
done
############  Here is where the Skimmer ends          ############



IFS="/"
set $DataSetName
OutName=$4$7$rootNumber_$PROCESS".root"  # this makes the 4th and 6th pieces of the
#OutName=$4$5$rootNumber_$PROCESS".root"
#OutName=$4$8$rootNumber_$PROCESS".root"
echo 'OutName== ' $OutName
hadd -f $OutName "skimed_"*.root


xrdcp $OutName  root://cmseos.fnal.gov//store/user/snandan/skim_test/HH_7thOcto



##########  remove the unneccesat files
cd ${_CONDOR_SCRATCH_DIR}
rm -rf CMSSW_8_0_25
echo "Done execution ..."

END_TIME=`/bin/date`
echo "finished at ${END_TIME}"

exit $exitcode
echo "DONE!"