TEXSOURCES= \
	../l2h.conf \
	../manual.tex 
HTMLOUTPUT= \
	*.html \
	*.gif \
	manual.css

DIR_CONTENTS= ${HTMLOUTPUT}
udehtmldocdir= ${prefix}/doc/${PACKAGE}-${VERSION}/html
udehtmldoc_DATA= ${DIR_CONTENTS}
EXTRA_DIST= ${DIR_CONTENTS}

manual.css: ${TEXSOURCES}
	ln -s ../manual.tex .
	latex2html -no_subdir -init_file ../l2h.conf manual.tex
	rm manual.tex

clean:
	rm -rf *.html *.gif *.pl *.css

*.html: manual.css
*.gif: manual.css
