#!/usr/bin/env python

import commands
import numpy
import sys
from string import replace

def get_count(cmd):
    #print cmd
    rc, out=commands.getstatusoutput(cmd)    
    counter=int(out.splitlines()[-1].split()[-1])
    #print "counter was %d" % (counter)
    return counter


def main():

    types = ['ALOCK', 'TASLOCK', 'TTASLOCK', 'BACKOFF']
    threadcounts = [2, 4, 8, 16, 32]

    latex = ''
    
    os = commands.getstatusoutput('uname -si')[1]
    cpus = open('/proc/cpuinfo').read().count('processor\t:')
    print 'machine : %s, %s CPUs' % (os, cpus)
    print ''

    latex += '\\subsection{%s, %s CPUs}\n\n' % (replace(os, '_', '\_'), cpus)
    latex += 'Results for running on %s with %s processors\n\n' % (replace(os, '_', '\_'), cpus)

    for type in types:
        latex += 'The following is the results for the %s lock.\n\n' % type
        latex += '\\begin{tabular}{|r||c|c|c|}\n'
        latex += ' \\hline Number of Threads & Number of Runs & Mean & Standard Deviation \\\\ \n'
        for threads in threadcounts:
            cmd = './second_test %s %s' % (type, threads)

            counts=[]
            n = 10
            for i in range(0,n):
                counts.append( get_count(cmd) )

            while ( n > len(counts) ) :
                counts.append( get_count(cmd) )

                dev  = numpy.std(counts)
                mean = numpy.mean(counts)
                n = ( (1.96 * dev ) / mean )

            print 'type : %s' % type
            print 'threads : %s' % threads
            print 'n : %s' % len(counts)
            print 'mean : %s' % numpy.mean(counts)
            print 'stddev : %s' % numpy.std(counts)
            print 'counts : %s' % counts
            print ''
            latex += ' \\hline %s & %s & %s & %s \\\\ \n' % (threads, n, numpy.mean(counts), numpy.std(counts))

        latex += '\\hline\n'
        latex += '\\end{tabular}\n\n\\vspace{12pt}'

    open('tex/analysis_sub.in.test', 'w+').write(latex)

if __name__ == "__main__":
    main()
