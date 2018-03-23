#! /usr/bin/env python

import os
import argparse
import json
import sys
#useful CMSSW json manipulation
from FWCore.PythonUtilities.LumiList import LumiList

def rm_hlt_version(name):
    version_start = name.rfind("_v")
    if version_start == -1: 
        return name
    else:
        return name[:version_start+2]    


def get_pathname_from_ps_tbl(entry):
    hlt_path = entry.split()[0]
    return rm_hlt_version(hlt_path)

def get_hlt_menu_version(hlt_menu):
    if hlt_menu.find("/cdaq/physics/Run2017/2e34/") !=0:
        print "error, hlt menu name",hlt_menu," not a 2017 collisions menu"
        return ""
    version = hlt_menu.split("/")[5]
    version = version[:version.rfind(".")]
    return version

def get_hlt_prescales(ps_tbl,pathname):
    for line in ps_tbl:
        if get_pathname_from_ps_tbl(line[1]) == pathname:
            return line
    return None

def get_l1_prescales(l1_ps_tbl,l1_seed):
    for line in l1_ps_tbl:
        if line[1] == l1_seed:
            return line
    return None

#they are anded together, using CMSSW json tools
def combine_grls(grl1,grl2):
    lumis1 = LumiList(compactList=grl1)
    lumis2 = LumiList(compactList=grl2)
    
    new_lumis = lumis1 & lumis2
    return new_lumis.compactList
        
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='splits a given good lumi json into prescaled and unprescaled components')
    parser.add_argument('--run_info',required=True,help='run info json')
    parser.add_argument('--ps_data',required=True,help='prescale data json') 
    parser.add_argument('--grl',required=True,help='good run list to split')
    parser.add_argument('--min_runnr',default=0,type=int,help='min run number')
    parser.add_argument('--max_runnr',default=999999,type=int,help='max run number')
    parser.add_argument('--path',default="HLT_DoubleEle33_CaloIdL_MW_v",help='HLT path')
    parser.add_argument('--isHLT',action='store_true',help="is a HLT path we are investigating")
    parser.add_argument('--isL1',action='store_true',help="is a L1 seed we are investigating")
    args = parser.parse_args()

    if args.isHLT==args.isL1:
        print "error, either --isHLT or --isL1 must be specificed but not both nor neither"
        sys.exit(0);

    #first load all the jsons
    good_lumis = {}
    with open(args.grl) as f:
        good_lumis = json.load(f)

    all_runs_info = {}
    with open(args.run_info) as f:
        all_runs_info = json.load(f)
    ps_data = {}
    with open(args.ps_data) as f:
        ps_data = json.load(f)

    runs = all_runs_info.keys()
    runs.sort()

    run_lumis_unprescaled = {}
    run_lumis_prescaled = {}
    
    for run in runs:

        run_info = all_runs_info[run]
        ps_tbl_key = ""
        if args.isHLT:
            ps_tbl_key = run_info["hlt_menu"] #HLT key
        else:
            ps_tbl_key = run_info["trig_mode"] #L1 key
        
        ps_tbl = ps_data[ps_tbl_key]
        
        if args.isHLT:
            path_prescales = get_hlt_prescales(ps_tbl,args.path)
        else:
            path_prescales = get_l1_prescales(ps_tbl,args.path)

        ps_cols = run_info['ps_cols']
        
        lumis_prescaled=[]
        lumis_unprescaled=[]

        for ps_col in ps_cols.keys():
            if int(ps_col) < 0: print "ps column is <0",run,ps_col
            ps_index = int(ps_col)+2
            if path_prescales==None or int(path_prescales[ps_index]) != 1:
                for lumis in ps_cols[ps_col]:
                    lumis_prescaled.append(lumis)
            else:
                for lumis in ps_cols[ps_col]:
                    lumis_unprescaled.append(lumis)

        if lumis_unprescaled != []:
            run_lumis_unprescaled[run] = lumis_unprescaled
        if lumis_prescaled != []:
            run_lumis_prescaled[run] = lumis_prescaled
    
    run_lumis_prescaled = combine_grls(run_lumis_prescaled,good_lumis)
    run_lumis_unprescaled = combine_grls(run_lumis_unprescaled,good_lumis)

    with open(args.path+'_prescaled.json', 'w') as outfile:
        json.dump(run_lumis_prescaled, outfile,sort_keys = True)
    with open(args.path+'_unprescaled.json', 'w') as outfile:
        json.dump(run_lumis_unprescaled, outfile,sort_keys = True)
        
    print args.path,"was prescaled for these lumisections:"
    print run_lumis_prescaled
    
