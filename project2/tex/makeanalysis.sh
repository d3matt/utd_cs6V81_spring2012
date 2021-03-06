#!/bin/sh

rm analysis.tex &> /dev/null

cat analysis_header.in > analysis.tex

echo '\section{Appendix}
Below is the data that was used to generate all the figures
' > analysis_appendix.in 

for dir in results.* ; do
    ( cd $dir ; ../plot.py )
done

for i in 1 2 3 ; do
    cat analysis_subheader$i.in >> analysis.tex
    for dir in results.* ; do
        ./makelatex$i.py $dir || echo $dir failed
    done

    for SUB in analysis_sub$i.in* ; do
        cat $SUB >> analysis.tex
    done

done

cat analysis_subheader4.in >> analysis.tex

cp analysis_appendix.in appendix.tex

