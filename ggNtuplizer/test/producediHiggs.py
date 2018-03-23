import os, sys, re

file = open("run_mc_80X1.py")
scriptLines = file.readlines()
file.close()

for i in range(1, 55):
    fileToCopy = "run_mc80XdiHiggs_" + str(i) + ".py"
    file = open(fileToCopy, "w")
    for line in scriptLines:
        if len(re.split("INPUT.ROOT", line)) > 1:
            newLine = line.replace("INPUT.ROOT", "root://cmseos.fnal.gov//store/user/snandan/HHTo4Tau/MiniAOD/MiniAOD_" + str(i) + ".root")
            file.write(newLine)
            continue
        if len(re.split("OUTPUT.ROOT", line)) > 1:
            newLine = line.replace("OUTPUT.ROOT", "file:ggtree_mc_" + str(i) + ".root")
            file.write(newLine)
            continue
        file.write(line)
    file.close()
