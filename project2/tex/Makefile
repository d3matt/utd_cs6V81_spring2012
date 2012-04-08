
all:
	$(HIDE) ./makelatex.py
	$(HIDE) ./makeanalysis.sh
	$(HIDE) ( pdflatex -jobname paper -halt-on-error -interaction batchmode project2.tex &> pdflatex.log ; echo 'done' > /dev/null )

clean:
	rm -f analysis.tex paper.log paper.aux paper.pdf
