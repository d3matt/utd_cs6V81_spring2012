#!/bin/sh

rm analysis.tex &> /dev/null

cat analysis_header.in > analysis.tex

for SUB in analysis_sub.in* ; do
    cat $SUB >> analysis.tex
done

