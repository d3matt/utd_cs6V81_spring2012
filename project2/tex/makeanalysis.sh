#!/bin/sh -x

rm analysis.tex &> /dev/null

cat analysis_header.in > analysis.tex

cat analysis_subheader1.in >> analysis.tex

for dir in results.* ; do
    ./makelatex1.py $dir || echo $dir failed
done

for SUB in analysis_sub1.in* ; do
    cat $SUB >> analysis.tex
done
