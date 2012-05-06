#!/usr/bin/env python

import commands
import json
import sys

def read_json(filename):
    jstring = open(filename).read()
    results = json.loads(jstring)
    return results

def gen_csv1(results, filename):

    fout = open("%s.csv" % (filename),"w")
    cpus = results['machine']['cpus']

    line = "#"
    for tt in results["types"]:
        line+=tt
        line+=","

    #pull off last comma
    line=line[:-1]
    line+='\n'

    fout.write(line)

    for tc in results["threadcounts"]:
        if tc > cpus:
            continue
        line = str(tc)
        line+=","

        for tt in results["types"]:
            line+=str(results[tt][str(tc)]["mean"])
            line+=","
        line=line[:-1]
        line+='\n'
        fout.write(line)

    fout.flush()
    fout.close()

POINTS = {
    0 : 4,
    1 : 8,
    2 : 10,
    3 : 12,
    4 : 14,
    5 : 16,
    6 : 19,
    7 : 27,
    8 : 28,
    9 : 29
}

def gen_plot1(results, filename):
    plot_string = """ \\
set key top left; \\
set key box; \\
set terminal pdf; \\
set out '%s.pdf'; \\
set datafile separator ','; \\
set logscale y; \\
set xlabel 'Number of Threads'; \\
set ylabel 'Stack operations';\\
plot """ % (
    filename)

    for i, tt in enumerate(results["types"]):
        plot_string += "'%s.csv' using 1:%d with linespoints pointtype %d title '%s', " % (
            filename, i+2, POINTS[i], tt)
    plot_string = plot_string[:-2]

    cmd = "gnuplot -e \"%s\"" % (plot_string)
    #print cmd
    rc, out = commands.getstatusoutput(cmd)
    #print out

def main():

    results = read_json("test_results")
    gen_csv1(results, "test_results")
    gen_plot1(results, "test_results")

if __name__ == "__main__":
    main()
