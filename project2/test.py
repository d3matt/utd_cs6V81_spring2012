#!/usr/bin/env python

import commands
import numpy
import sys

def get_count(cmd):
    #print cmd
    rc, out=commands.getstatusoutput(cmd)    
    counter=int(out.splitlines()[-1].split()[-1])
    #print "counter was %d" % (counter)
    return counter


def main():

    types = ['ALOCK', 'TASLOCK', 'TTASLOCK', 'BACKOFF']
    threadcounts = [2, 4, 8, 16, 32]

    for type in types:
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

if __name__ == "__main__":
    main()
