#!/usr/bin/env python

import subprocess

execs = ['simple_test', 'second_test']
types = ['ALOCK', 'TASLOCK', 'TTASLOCK', 'BACKOFF', 'PTHREAD']
threadcounts = [1, 2, 10, 64, 75, 100]

for execname in execs:
    for type in types:
        for threads in threadcounts:
            subprocess.call(['./%s.prof' % execname , str(type), str(threads)])
            process = subprocess.Popen(['gprof', '%s.prof' % execname], stdout=subprocess.PIPE)
            output = process.communicate()[0]

            filename = '.profile/%s.profile.%s.%s' % (execname, type, threads)
            open(filename, 'w+').write(output)

