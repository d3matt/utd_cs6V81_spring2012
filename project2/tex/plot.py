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
            line+=str(results["noyield"][tt][str(tc)]["mean"])
            line+=","
        line=line[:-1]
        line+='\n'
        fout.write(line)

    fout.flush()
    fout.close()

def gen_csv2(results, filename):

    fout = open("%s.csv" % (filename),"w")

    line = "#"
    for min in results["mindelays"]:
        line+=str(min)
        line+=","

    #pull off last comma
    line=line[:-1]
    line+='\n'

    fout.write(line)

    for max in results["maxdelays"]:
        line = str(max)
        line+=","
        for min in results["mindelays"]:
            if max < min:
                continue

            line+=str(results[str(min)][str(max)]["mean"])
            line+=","

        line=line[:-1]
        line+='\n'
        fout.write(line)

    fout.flush()
    fout.close()

def gen_csv3(results, filename):

    for t in results["types"]:
        if t in ['BACKOFF', 'PTHREAD']:
            continue
        fout = open("%s-%s.csv" % (filename, t), "w")

        line = "#With Yield,Without Yield\n"

        fout.write(line)

        for tc in results["threadcounts"]:
            line = str(tc)
            line+=","

            line += str(results["yield"][t][str(tc)]["mean"]) + "," + str(results["noyield"][t][str(tc)]["mean"]) + "\n"
            fout.write(line)

        fout.flush()
        fout.close()

        gen_plot3(results, "%s-%s" % (filename, t))

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
set ylabel 'Time in lock (ns)';\\
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

def gen_plot2(results, filename):
    plot_string = """ \\
set key top left; \\
set key box; \\
set terminal pdf; \\
set out '%s.pdf'; \\
set datafile separator ','; \\
set xlabel 'Max Delay (us)'; \\
set ylabel 'Time in lock() (ns)';\\
plot """ % (
    filename)

    for i, min in enumerate(results["mindelays"]):
        plot_string += "'%s.csv' using 1:%d with linespoints pointtype %d title '%s', " % (
            filename, i+2, POINTS[i], min)
    plot_string = plot_string[:-2]

    cmd = "gnuplot -e \"%s\"" % (plot_string)
    #print cmd
    rc, out = commands.getstatusoutput(cmd)
    #print out

def gen_plot3(results, filename):
    plot_string = """ \\
set key top left; \\
set key box; \\
set terminal pdf; \\
set out '%s.pdf'; \\
set datafile separator ','; \\
set logscale y; \\
set xlabel 'Number of Threads'; \\
set ylabel 'Time in lock (ns)';\\
plot """ % (
    filename)

    for i, tt in enumerate(["Without Yield", "With Yield"]):
        plot_string += "'%s.csv' using 1:%d with linespoints pointtype %d title '%s', " % (
            filename, i+2, POINTS[i], tt)
    plot_string = plot_string[:-2]

    cmd = "gnuplot -e \"%s\"" % (plot_string)
    #print cmd
    rc, out = commands.getstatusoutput(cmd)
    #print out

def main():

    results = read_json("test_results")
    #Make a plot of the different lock types using pthread_yield()
    gen_csv1(results, "test_results")
    gen_plot1(results, "test_results")

    #do 3 first so i don't have to re-read results
    #Plot the difference when using pthread_yield() and not using it
    gen_csv3(results, "test_results")
    #in this case, gen_csv calls gen_plot

    results = read_json("backoff_results")
    #Plot the different results when varying MINDELAY and MAXDELAY
    gen_csv2(results, "backoff_results")
    gen_plot2(results, "backoff_results")
    

if __name__ == "__main__":
    main()
