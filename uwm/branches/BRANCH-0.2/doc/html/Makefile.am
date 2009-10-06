TEXSOURCES= \
	../l2h.conf \
	./manual.tex 
HTMLOUTPUT= \
	*.html \
	*.gif \
	manual.css

DIR_CONTENTS= ${HTMLOUTPUT}
udehtmldocdir= ${prefix}/doc/${PACKAGE}-${VERSION}/html
udehtmldoc_DATA= ${DIR_CONTENTS}
EXTRA_DIST= ${DIR_CONTENTS}

manual.tex: ../manual.tex
	rm -f ./manual.tex
	ln -s ../manual.tex .

manual.css: ${TEXSOURCES}
	rm -f manual.css
	latex2html -no_subdir -init_file ../l2h.conf manual.tex

clean-local:
	rm -rf *.html *.gif *.pl *.css

*.html: manual.css
*.gif: manual.css
