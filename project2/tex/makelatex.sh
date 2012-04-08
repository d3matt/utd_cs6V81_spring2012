#!/bin/sh

rm analysis.tex

cat analysis_header.in > analysis.tex

for SUB in analysis_sub.in* ; do
    cat $SUB >> analysis.tex
done

