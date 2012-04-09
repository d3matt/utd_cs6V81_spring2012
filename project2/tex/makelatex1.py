#!/usr/bin/env python

import sys
import string

filename = 'test_results'
try:
    filename = sys.argv[1]
except:
    pass

try:
    results = open(filename, 'r').read().splitlines()
except():
    sys.exit(0)
latex = ''


machine = results[0].split(':')[1].strip()
types = int(results[1].split(':')[1].strip())
threadcounts = int(results[2].split(':')[1].strip())


results = results[4:]

latex += '\\subsubsection{%s}\n\n' % (string.replace(machine, '_', '\_'))
latex += 'Results in tables '
for i in range(0,types):
    latex += '\\ref{table_%s_%s}' % (filename, i)
    if i != types-1:
        latex += ', '
latex += '\n\n'

for i in range(0,types):
    type = results[0].split(':')[1].strip()
    latex += '\\begin{table}[hp]\n'
    latex += ' \\caption{%s lock}\n' % type
    latex += ' \\begin{center}\n'
    latex += ' \\begin{tabular}{|r||c|c|c|}\n'
    latex += '  \\hline Number of Threads & Number of Runs & Mean & Standard Deviation \\\\ \n'
    for j in range(0,threadcounts):
        threads = results[1].split(':')[1].strip()
        n = results[2].split(':')[1].strip()
        mean = results[3].split(':')[1].strip()
        stddev = results[4].split(':')[1].strip()

        latex += '  \\hline %s & %s & %s & %s \\\\ \n' % (threads, n, mean, stddev)
        results = results[6:]
    latex += '  \\hline\n'
    latex += ' \\end{tabular}\n'
    latex += ' \\end{center}\n'
    latex += ' \\label{table_%s_%s}\n' % (filename, i)
    latex += '\\end{table}\n\n'

latex += '\\clearpage\n\n'

open('analysis_sub1.in.%s' % filename, 'w+').write(latex)

