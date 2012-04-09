#!/bin/sh

rm analysis.tex &> /dev/null

cat analysis_header.in > analysis.tex

cat analysis_subheader1.in >> analysis.tex

for file in test_results* ; do
    ./makelatex1.py $file
done

for SUB in analysis_sub1.in* ; do
    cat $SUB >> analysis.tex
done

