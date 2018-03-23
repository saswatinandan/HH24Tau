# This script is used to catch a list of all files in the /eos to be used for Skimming
# python snagSample.py  File1.txt   File2.txt
# This script needs to be repeat several times untill we get to the last directory of the eos which all the root files exist
#The last txt file should be renamed as InputSamples.txt 
import os
import sys
os.system('cat  %s | xargs -n 1 -I {} echo "xrdfs root://cmseos.fnal.gov ls {}" > %s'%(sys.argv[1],"tmp.txt" ))
os.system('bash %s > %s'%("tmp.txt", sys.argv[2]))
