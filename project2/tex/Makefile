
all:
	$(HIDE) ./makelatex.sh
	$(HIDE) ( pdflatex -jobname paper -halt-on-error -interaction batchmode project2.tex &> pdflatex.log ; echo 'done' > /dev/null )
