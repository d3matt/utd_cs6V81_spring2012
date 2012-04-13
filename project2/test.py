#!/usr/bin/env python

import commands
import json
import numpy
import sys
from string import replace

def get_count(cmd):
    #print cmd
    rc, out = commands.getstatusoutput(cmd)
    counter = 0
    total   = 0
    for line in out.splitlines():
        if "counter" in line:
            counter = int( line.split()[-1])
            continue
        if "total_nsec" in line:
            total = int( line.split()[-1] )
            continue

    return total / float(counter)

results={}

def dot():
    sys.stdout.write(".")
    sys.stdout.flush()

def main():

    types = ['TASLOCK', 'TTASLOCK', 'BACKOFF', 'ALOCK', 'PTHREAD']
    threadcounts = [1, 2, 10, 32, 64, 75, 100, 128]

    print "Running..."
    
    os = commands.getstatusoutput('uname -si')[1]
    cpuinfo = open('/proc/cpuinfo').read()
    cpus = cpuinfo.count('processor\t:')
    for line in cpuinfo.splitlines():
        if "model name" in line:
            model = line.split(':', 1)[1].strip()
            break

    results['machine'] = { "os":os, "cpus":cpus, "model": model}
    results['types'] = types
    results['threadcounts'] = threadcounts

    for YIELD,suffix in zip(['noyield', 'yield'], ['', '.yield']):

        results[YIELD] = {}

        for ltype in types:

            results[YIELD][ltype]={}

            for threads in threadcounts:
                results[YIELD][ltype][threads]={}
                cmd = './second_test%s %s %s' % (suffix, ltype, threads)
                print cmd

                counts=[]
                n = 10
                for i in range(0,n):
                    counts.append( get_count(cmd) )
                    dot()
                print

                #loop was always true...
                dev  = numpy.std(counts)
                mean = numpy.mean(counts)
                n = ( (1.96 * dev ) / mean )
                while ( n > len(counts) ) :
                    counts.append( get_count(cmd) )

                    dev  = numpy.std(counts)
                    mean = numpy.mean(counts)
                    n = ( (1.96 * dev ) / mean )

                results[YIELD][ltype][threads]["n"] = len(counts)
                results[YIELD][ltype][threads]["mean"] = numpy.mean(counts)
                results[YIELD][ltype][threads]["stddev"] = numpy.std(counts)
                results[YIELD][ltype][threads]["counts"] = counts

                print '   type : %s' % ltype
                print 'threads : %s' % threads
                print '      n : %s' % len(counts)
                print '   mean : %s' % numpy.mean(counts)
                print ' stddev : %s' % numpy.std(counts)
                print ' counts : %s' % counts


    jstring = json.dumps(results, indent=4)
    open('test_results', 'w+').write(jstring)
    print 'Done'

if __name__ == "__main__":
    main()
