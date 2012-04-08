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

    log = ''
    print "Running..."
    
    os = commands.getstatusoutput('uname -si')[1]
    cpus = open('/proc/cpuinfo').read().count('processor\t:')

    log += 'machine : %s, %s CPUs\n' % (os, cpus)
    log += 'types : %s\n' % (len(types))
    log += 'threadcounts : %s\n' % (len(threadcounts))
    log += '\n'

    for type in types:

        for threads in threadcounts:
            cmd = './second_test %s %s' % (type, threads)

            counts=[]
            n = 10
            for i in range(0,n):
                counts.append( get_count(cmd) )
                print ".",

            #loop was always true...
            dev  = numpy.std(counts)
            mean = numpy.mean(counts)
            n = ( (1.96 * dev ) / mean )
            while ( n > len(counts) ) :
                counts.append( get_count(cmd) )

                dev  = numpy.std(counts)
                mean = numpy.mean(counts)
                n = ( (1.96 * dev ) / mean )

            log += 'type : %s\n' % type
            log += 'threads : %s\n' % threads
            log += 'n : %s\n' % len(counts)
            log += 'mean : %s\n' % numpy.mean(counts)
            log += 'stddev : %s\n' % numpy.std(counts)
            log += 'counts : %s\n' % counts

            print

    open('tex/test_results.log', 'w+').write(log)
    print 'Done'

if __name__ == "__main__":
    main()
