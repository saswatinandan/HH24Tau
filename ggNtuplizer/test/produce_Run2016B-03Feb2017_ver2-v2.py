import os, sys, re

file = open("W.txt")
scriptLines = file.readlines()
file1 = open("run_mc_80X1.py")
scriptLines1 = file1.readlines()

file1.close()

for i, line in enumerate(scriptLines):
    newLine = line
    fileToCopy = "run_mc80X_W_" + str(i) + ".py"
    file1 = open(fileToCopy, "w")
    for line1 in scriptLines1 :
        if len(re.split("INPUT.ROOT", line1)) > 1 :
            newLine = line1.replace('"INPUT.ROOT"','"root://cmsxrootd.fnal.gov///'+ newLine)
            newLine = newLine.replace("'","")
            newLine = newLine.replace(",",'"')
            file1.write(newLine)
            continue
        if len(re.split("OUTPUT.ROOT", line1)) > 1 :
            newLine = line1.replace("OUTPUT.ROOT", "ggtree_mc_"+str(i)+ ".root")
            file1.write(newLine)
            continue
        file1.write(line1)
    file1.close()



