
all: analysis.tex
	$(HIDE) ( pdflatex -jobname paper -halt-on-error -interaction batchmode project2.tex &> pdflatex.log ; echo 'done' > /dev/null )
	$(HIDE) ( pdflatex -jobname paper -halt-on-error -interaction batchmode project2.tex &> pdflatex.log ; echo 'done' > /dev/null )

analysis.tex: 
	$(HIDE) ./makeanalysis.sh

.PHONY: pdf

pdf: analysis.tex
	$(HIDE) ( pdflatex -jobname paper -halt-on-error -interaction batchmode project2.tex &> pdflatex.log ; echo 'done' > /dev/null )
	$(HIDE) ( pdflatex -jobname paper -halt-on-error -interaction batchmode project2.tex &> pdflatex.log ; echo 'done' > /dev/null )

clean:
	rm -f analysis.tex analysis_sub{1,2,3}.in* *.log *.aux *.pdf
	rm -f results.*/*.pdf results.*/*.csv
