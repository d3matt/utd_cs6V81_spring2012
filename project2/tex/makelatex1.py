#!/usr/bin/env python

import json
import os
import sys

filename = ''
dirname = ''
specifier = ''
try:
    dirname = sys.argv[1]
    specifier = sys.argv[1].split('.')[-1]
except:
    filename = 'test_results'

try:
    jstring = ''
    if dirname == '':
        jstring = open(filename, 'r').read()
    else:
        jstring = open('%s/test_results' % dirname, 'r').read()
    results = json.loads( jstring )
except:
    sys.exit(0)
latex = ''

types = results["types"]

latex += '\\subsubsection{%s, %d CPUs : %s}\n\n' % (
    results["machine"]["os"].replace('_', '\_'),
    results["machine"]["cpus"],
    results["machine"]["model"] )
latex += 'Results in tables '
for i in range(0,len(types)):
    latex += '\\ref{table_%s_%s}' % (specifier, i)
    if i != len(types)-1:
        latex += ', '
latex += '\n\n'

#pretty sure type is a reserved word
for i,ltype in enumerate(types):
    if os.path.exists('%s/%s.pdf' % (dirname, ltype)):
        latex += '\\begin{figure}[hp]\n'
        latex += ' \\caption{Time in lock() vs. Number of Threads}\n'
        latex += ' \\begin{center}\n'
        latex += '  \\includegraphics{%s/%s.pdf}\n' % (dirname, ltype)
        latex += ' \\end{center}\n'
        latex += ' \\label{figure_%s_%s}\n' % (specifier, i)
        latex += '\\end{figure}\n'
    latex += '\\begin{table}[hp]\n'
    latex += ' \\caption{%s lock}\n' % ltype
    latex += ' \\begin{center}\n'
    latex += ' \\begin{tabular}{|r||c|c|c|}\n'
    latex += '  \\hline Number of Threads & Number of Runs & Mean & Standard Deviation \\\\ \n'
    for threads in results["threadcounts"]:
        ts = "%d" % (threads)
        n = results[ltype][ts]["n"]
        mean = results[ltype][ts]["mean"]
        stddev = results[ltype][ts]["stddev"]

        latex += '  \\hline %s & %s & %s & %s \\\\ \n' % (threads, n, mean, stddev)
    latex += '  \\hline\n'
    latex += ' \\end{tabular}\n'
    latex += ' \\end{center}\n'
    latex += ' \\label{table_%s_%s}\n' % (specifier, i)
    latex += '\\end{table}\n\n'

latex += '\\clearpage\n\n'

open('analysis_sub1.in.%s' % filename, 'w+').write(latex)

