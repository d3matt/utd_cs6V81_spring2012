#!/usr/bin/env python

import commands
import sys

mindelays = [1, 2, 4, 8, 16, 32]
maxdelays = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512]

for min in mindelays:
    for max in maxdelays:
        if max < min:
            pass
        else:
            rc, output = commands.getstatusoutput('./second_test BACKOFF MINDELAY=%s MAXDELAY=%s 6' % (min, max))
            counter = int(output.splitlines()[-1].split()[-1])
            print 'min: %s, max: %s, counter: %s' % (min, max, counter)


