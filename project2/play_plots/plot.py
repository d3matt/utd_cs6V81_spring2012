#!/usr/bin/env python

import commands
import json
import sys

def read_json(filename):
    jstring = open(filename).read()
    results = json.loads(jstring)
    return results

def gen_csv(results, filename):

    fout = open("%s.csv" % (filename),"w")

    line = "#"
    for tt in results["types"]:
        line+=tt
        line+=","

    #pull off last comma
    line=line[:-1]
    line+='\n'

    fout.write(line)

    for tc in results["threadcounts"]:
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
    2 : 4,
    3 : 8,
    4 : 10,
    5 : 12,
    6 : 14
}

def gen_plot(results, filename):
    plot_string = """ \\
set title 'average time in lock() on %s, %d CPUs'; \\
set key top left; \\
set key box; \\
set terminal pdf; \\
set out '%s.pdf'; \\
set datafile separator ','; \\
set xlabel 'threads'; \\
set ylabel 'time in lock (ns)';\\
plot """ % (
    results["machine"]["os"],
    results["machine"]["cpus"],
    filename)

    i = 2
    for tt in results["types"]:
        plot_string += "'%s.csv' using 1:%d with linespoints pointtype %d title '%s', " % (
            filename, i, POINTS[i], tt)
        i = i + 1
    plot_string = plot_string[:-2]

    cmd = "gnuplot -e \"%s\"" % (plot_string)
    print cmd
    rc, out = commands.getstatusoutput(cmd)
    print out



FILENAME="test_results"

def main():
    results = read_json(FILENAME)
    gen_csv(results, FILENAME)
    gen_plot(results, FILENAME)

if __name__ == "__main__":
    main()
