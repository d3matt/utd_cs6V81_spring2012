#!/usr/bin/env python

"""Test the project..."""

import commands
import json
import numpy
import os
import sys
from subprocess import check_output

def dump_os():
    (sysname, nodename, release, version, machine) = os.uname()
    return "%s %s" % (sysname, machine)

def dot():
    sys.stdout.write(".")
    sys.stdout.flush()

def run(qtype, threads):
    output = check_output(['./testtwo', qtype, 'NUMTHREADS=%d' % threads])
    return float(output.split()[0])
    
    
    
def main():
    results = {}
    types = [ 'LOCK', 'LOCKFREE', 'ELIMINATION' ]
    threadcounts = [ 6, 10 ]

    print "Running..."
    
    cpuinfo = open('/proc/cpuinfo').read()
    cpus = cpuinfo.count('processor\t:')
    for line in cpuinfo.splitlines():
        if "model name" in line:
            model = line.split(':', 1)[1].strip()
            break

    for x in range(cpus):
        threadcounts.append(x+1)
    #remove duplicates
    threadcounts = list(set(threadcounts))
    threadcounts.sort()


    results['machine'] = { "os":dump_os(), "cpus":cpus, "model": model}
    results['types'] = types
    results['threadcounts'] = threadcounts

    for qtype in types:
        results[qtype] = {}
        for threads in threadcounts:
            counts = []
            n = 10
            print "%s %s" % (qtype, threads)
            for i in range(0,n):
                counts.append( run(qtype, threads) )
                dot()
            dev  = numpy.std(counts)
            mean = numpy.mean(counts)
            n = ( (1.96 * dev ) / mean )
            while ( n > len(counts) ) :
                counts.append( run(qtype, threads) )
                dev  = numpy.std(counts)
                mean = numpy.mean(counts)
                n = ( (1.96 * dev ) / mean )

            z = {}
            z["n"] = len(counts)
            z["mean"] = numpy.mean(counts)
            z["stddev"] = numpy.std(counts)
            z["counts"] = counts

            results[qtype][threads] = z

            print '   type : %s' % qtype
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
