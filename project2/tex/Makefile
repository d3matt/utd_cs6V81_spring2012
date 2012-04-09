
all:
	$(HIDE) ./makeanalysis.sh
	$(HIDE) ( pdflatex -jobname paper -halt-on-error -interaction batchmode project2.tex &> pdflatex.log ; echo 'done' > /dev/null )

clean:
	rm -f analysis.tex analysis_sub1.in* *.log *.aux *.pdf
