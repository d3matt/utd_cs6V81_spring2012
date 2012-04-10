#!/usr/bin/env python

import commands
import json
import numpy
import sys

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

def dot():
    sys.stdout.write(".")
    sys.stdout.flush()

results={}

mindelays = [1, 2, 4, 8, 16, 32]
maxdelays = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512]

for min in mindelays:
    results[min] = {}
    for max in maxdelays:
        if max < min:
            pass
        else:
            results[min][max] = {}
            counts = []
            cmd = './second_test BACKOFF MINDELAY=%s MAXDELAY=%s 512' % (min, max)
            print cmd
            for i in range(0,10):
                counts.append(get_count(cmd))
                dot()
            print

            print 'min: %s' % min
            print 'max: %s' % max
            print 'mean: %s' % numpy.mean(counts)
            print 'stddev: %s' % numpy.std(counts)
            print 'counts: %s' % counts
            results[min][max]["min"] = min
            results[min][max]["max"] = max
            results[min][max]["n"] = len(counts)
            results[min][max]["mean"] = numpy.mean(counts)
            results[min][max]["stddev"] = numpy.std(counts)
            results[min][max]["counts"] = counts


open('backoff_results', 'w').write(json.dumps(results, indent=4))
