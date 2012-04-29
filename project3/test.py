#!/usr/bin/env python

"""Test the project..."""

import subprocess

for threads in [ 1, 2, 6, 10 ]:
    subprocess.call(['./testone', 'ELIMINATION', 'NUMTHREADS=%s' % threads])

