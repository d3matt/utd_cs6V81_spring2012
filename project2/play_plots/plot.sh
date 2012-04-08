#!/bin/bash
gnuplot -e "set title 'throughput versus threads'; \
		set key top left; \
		set key box; \
		set terminal png; \
		set out 'ALOCK.PNG'; \
		set datafile separator ','; \
		set xlabel 'threads'; \
		set ylabel 'throughput';\
		plot \
		'ALOCK.CSV' using 1:2:3 with errorbars pointtype 2 title 'ALOCK' \
        "

gnuplot -e "set title 'throughput versus threads'; \
		set key top left; \
		set key box; \
		set terminal png; \
		set out 'ALOCK2.PNG'; \
		set datafile separator ','; \
		set xlabel 'threads'; \
		set ylabel 'throughput';\
		plot \
		'ALOCK.CSV' using 1:2:3 with linespoints pointtype 2 title 'ALOCK' \
        "
