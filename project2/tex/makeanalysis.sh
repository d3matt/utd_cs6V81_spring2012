#!/bin/sh

rm analysis.tex &> /dev/null

cat analysis_header.in > analysis.tex


for dir in results.* ; do
    ( cd $dir ; ../plot.py )
done

for i in 1 2 ; do
    cat analysis_subheader$i.in >> analysis.tex
    for dir in results.* ; do
        ./makelatex$i.py $dir || echo $dir failed
    done

    for SUB in analysis_sub$i.in* ; do
        cat $SUB >> analysis.tex
    done

done
