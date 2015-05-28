#!/usr/bin/env python2

import os
import os.path
import sys
import glob
from multiprocessing import Pool
import subprocess
import time

def hadd(in_files, out_file):
    subprocess.call(["hadd", out_file] + in_files)
    return 0

def main():
    verbose = False
    in_dir = "../user.nmorange.mc15_13TeV.410000.PowhegPythiaEvtGen_nonallhad.merge.NTUP.e3698_s2608_s2183_r6630_r6264_v2_BTAGSTREAM/"
    out_dir = "../user.nmorange.mc15_13TeV.410000.PowhegPythiaEvtGen_nonallhad.merge.NTUP.e3698_s2608_s2183_r6630_r6264_v2_merged/"
    nfiles_per_job = 50
    output_files_prefix = "ttbar"
    n_processes = 8

    if len(sys.argv)>1:
        in_dir = sys.argv[1]
    if len(sys.argv)>2:
        out_dir = sys.argv[2]
    if len(sys.argv)>3:
        nfiles_per_job = sys.argv[3]
    if len(sys.argv)>4:
        output_files_prefix = sys.argv[4]

    # check that input exists
    if not os.path.exists(in_dir):
        print "ERROR: input directory", in_dir, "not found"
        return

    in_dir = os.path.abspath(in_dir)
    out_dir = os.path.abspath(out_dir)

    # create output dir
    try:
        os.makedirs(out_dir)
    except:
        pass

    in_files = glob.glob(in_dir+"/*.root*")
    print "We found", len(in_files), "files to hadd"
    if len(in_files) == 0:
        return 0
    in_files.sort()
    if verbose:
        print in_files

    # compute the number of jobs to do
    n_jobs = (len(in_files)-1)/nfiles_per_job + 1

    # create the pool of processes
    pool = Pool(processes=n_processes)
    for i in range(n_jobs):
        out_file = "{0}/{1}_{2:03}.root".format(out_dir, output_files_prefix, i)
        time.sleep(1)
        pool.apply_async(hadd, (in_files[i*nfiles_per_job:(i+1)*nfiles_per_job], out_file))
    pool.close()
    pool.join()

    return 0

if __name__ == "__main__":
    main()
