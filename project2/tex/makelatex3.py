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
    filename = 'backoff_results'


try:
    jstring = ''
    if dirname == '':
        jstring = open(filename, 'r').read()
    else:
        jstring = open('%s/backoff_results' % dirname, 'r').read()
    results = json.loads( jstring )
except:
    sys.exit(1)
latex = ''

latex += '\\subsubsection{%s, %d CPUs : %s}\n\n' % (
    results["machine"]["os"].replace('_', '\_'),
    results["machine"]["cpus"],
    results["machine"]["model"] )

if os.path.exists('%s/backoff_results.pdf' % dirname):
    latex += 'An overview graph is included in Figure \\ref{figure:backoff_%s}.  This graph summarizes the performance of BACKOFF lock as the MIN and MAX delays vary.\n' % (specifier)
    latex += '\\begin{figure}[hp]\n'
    latex += ' \\caption{Time in lock() vs. MAX Value (MIN value is each plot)}\n'
    latex += ' \\begin{center}\n'
    latex += '  \\includegraphics{%s/backoff_results.pdf}\n' % (dirname)
    latex += ' \\end{center}\n'
    latex += ' \\label{figure:backoff_%s}\n' % (specifier)
    latex += '\\end{figure}\n\n'

latex += 'Data in table \\ref{table_%s_backoff}\n\n' % (specifier)

latex += '\\clearpage\n\n'
open('analysis_sub3.in.%s' % specifier, 'w+').write(latex)

latex = ''

latex += '\\begin{table}[hp]\n'
latex += ' \\caption{Backoff Results}\n'
latex += ' \\begin{center}\n'
latex += ' \\begin{tabular}{|r||c|c|}\n'
latex += '  \\hline Min Delay,Max Delay & Mean & Standard Deviation \\\\ \n'
for min in results["mindelays"]:
    for max in results["maxdelays"]:
        if max < min:
            continue

        mins = "%s" % min
        maxs = "%s" % max
        mean = results[mins][maxs]["mean"]
        stddev = results[mins][maxs]["stddev"]

        latex += '  \\hline %s,%s & %s & %s \\\\ \n' % (min, max, mean, stddev)
latex += '  \\hline\n'
latex += ' \\end{tabular}\n'
latex += ' \\end{center}\n'
latex += ' \\label{table_%s_backoff}\n' % (specifier)
latex += '\\end{table}\n\n'

open('analysis_appendix.in', 'a').write(latex)
