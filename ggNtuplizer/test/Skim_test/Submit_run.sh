#!/bin/bash
#
echo "input parameters: cluster, process, run path, out path, sample name" $1 $2 $3 $4
CLUSTER=$1
PROCESS=$2
RUNPATH=$3
OUTPATH=$4


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
echo "RUNPATH: $RUNPATH"
echo "OUTPATH: $OUTPATH"

source /cvmfs/cms.cern.ch/cmsset_default.sh
echo $PWD
cd $RUNPATH
eval `scram runtime -sh`
echo $PWD , "for cmsenv"

echo "check whether eos is working here"
echo ${_CONDOR_SCRATCH_DIR}
cd ${_CONDOR_SCRATCH_DIR}
echo $PWD , "for job running"


###### Copy the neccessary files for running the Skim
cp $RUNPATH/Skimmer.cc .
cp $RUNPATH/Skimmer.h .
cp $RUNPATH/Makefile .
cp $RUNPATH/l.txt .





#########  Smaple/Job splitting
SplitingNumber=10
DataSetArray=($(cat l.txt)) # array of the input datadets
echo ${DataSetArray[$PROCESS / $SplitingNumber]}
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
rm *root  Skimmer.cc  Skimmer.h  Makefile  l.txt
echo "Done execution ..."

END_TIME=`/bin/date`
echo "finished at ${END_TIME}"

exit $exitcode
echo "DONE!"