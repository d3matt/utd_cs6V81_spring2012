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
    sys.exit(1)
latex = ''

types = results["types"]

latex += '\\subsubsection{%s, %d CPUs : %s}\n\n' % (
    results["machine"]["os"].replace('_', '\_'),
    results["machine"]["cpus"],
    results["machine"]["model"] )


latex += 'Data in tables '
for ltype in types:
    if os.path.exists('%s/test_results-%s.pdf' % (dirname, ltype)):
        latex += '\\ref{table:yield_%s_%s}' % (specifier, ltype)
        latex += ', '

#remove ', '
latex = latex[:-2]
latex += '\n\n'

for ltype in types:
    if os.path.exists('%s/test_results-%s.pdf' % (dirname, ltype)):
        latex += 'An overview graph is included in Figure \\ref{figure:yield_%s_%s}.  This graph summarizes the performance of %s with and without \\verb+pthread_yield()+ as the number of threads increases.\n' % (specifier, ltype, ltype)
        latex += '\\begin{figure}[hp]\n'
        latex += ' \\caption{Time in lock() for %s vs. Number of Threads}\n' % ltype
        latex += ' \\begin{center}\n'
        latex += '  \\includegraphics{%s/test_results-%s.pdf}\n' % (dirname, ltype)
        latex += ' \\end{center}\n'
        latex += ' \\label{figure:yield_%s_%s}\n' % (specifier, ltype)
        latex += '\\end{figure}\n\n'
        latex += '\\begin{table}[hp]\n'
        latex += ' \\caption{%s lock}\n' % ltype
        latex += ' \\begin{center}\n'
        latex += ' \n\nWith \\verb+pthread_yield()+\n\n'
        latex += ' \\begin{tabular}{|r||c|c|c|}\n'
        latex += '  \\hline Number of Threads & Number of Runs & Mean & Standard Deviation \\\\ \n'
        for threads in results["threadcounts"]:
            ts = "%d" % (threads)
            n = results["yield"][ltype][ts]["n"]
            mean = results["yield"][ltype][ts]["mean"]
            stddev = results["yield"][ltype][ts]["stddev"]
            latex += '  \\hline %s & %s & %s & %s \\\\ \n' % (threads, n, mean, stddev)
        latex += '  \\hline\n'
        latex += ' \\end{tabular}\n'
        latex += ' \n\nWithout \\verb+pthread_yield()+\n'
        latex += ' \\begin{tabular}{|r||c|c|c|}\n'
        latex += '  \\hline Number of Threads & Number of Runs & Mean & Standard Deviation \\\\ \n'
        for threads in results["threadcounts"]:
            ts = "%d" % (threads)
            n = results["noyield"][ltype][ts]["n"]
            mean = results["noyield"][ltype][ts]["mean"]
            stddev = results["noyield"][ltype][ts]["stddev"]
            latex += '  \\hline %s & %s & %s & %s \\\\ \n' % (threads, n, mean, stddev)
        latex += '  \\hline\n'
        latex += ' \\end{tabular}\n'
        latex += ' \\end{center}\n'
        latex += ' \\label{table:yield_%s_%s}\n' % (specifier, ltype)
        latex += '\\end{table}\n\n'

latex += '\\clearpage\n\n'

open('analysis_sub2.in.%s' % dirname, 'w+').write(latex)

