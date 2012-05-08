
.PHONY: all

all: pdf

.PHONY: pdf

pdf: plot
	$(HIDE) ( pdflatex -jobname paper -halt-on-error -interaction batchmode project3.tex &> pdflatex.log ; echo 'done' > /dev/null )
	$(HIDE) ( pdflatex -jobname paper -halt-on-error -interaction batchmode project3.tex &> pdflatex.log ; echo 'done' > /dev/null )

plot: plot.py makeplots.sh
	$(HIDE) ./makeplots.sh

.PHONY: clean

clean:
	rm -f analysis.tex analysis_sub{1,2,3}.in* *.log *.aux *.pdf analysis_appendix.in appendix.tex
	rm -f results.*/*.pdf results.*/*.csv
