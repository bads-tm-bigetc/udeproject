TEXSOURCES= \
	uwm_manual.lyx \
	scratch.lyx \
	l2h.conf \
	manual.tex 
TEXOUTPUT= \
	manual.dvi

SUBDIRS= html
DIR_CONTENTS= ${TEXSOURCES} ${TEXOUTPUT} README
udedocdir= ${prefix}/doc/${PACKAGE}-${VERSION}
udedoc_DATA= ${DIR_CONTENTS}
EXTRA_DIST= ${DIR_CONTENTS}

manual.dvi: manual.tex
	latex manual.tex
	latex manual.tex
	latex manual.tex

clean:
	rm -rf *.log *.aux *.toc *.dvi
