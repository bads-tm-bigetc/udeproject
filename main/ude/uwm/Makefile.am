bin_PROGRAMS= uwm
# When the library be ready
# uwm_LDADD= ../lib/libude.la
uwm_LDFLAGS= ${X_LIBS} ${X_EXTRA_LIBS} ${X_PRE_LIBS}
# I think this should be diferent from the ude version.
INCLUDES= ${X_CFLAGS} -I../lib -DUWMVERSION=\"@VERSION@\" \
	-DLOCALEDIR=\"${datadir}/locale\" -DUDE_DIR=\"${pkgdatadir}\" \
	-DUDE_BIN_PATH=\"${bindir}\"
uwm_SOURCES=  \
    confparse.lex.c \
    confparse.tab.c \
    widgets.c \
    special.c\
    rubber.c\
    move.c \
    resize.c \
    windows.c \
    handlers.c \
    properties.c \
    nodes.c \
    winmenu.c \
    winmenumenu.c \
    menu.c \
    applications.c \
    init.c \
    uwmmenu.c \
    placement.c \
    urm.c \
    workspaces.c \
    pix.c \
    selection.c \
    wingroups.c \
    tree.c \
    configconvertors.c \
    uwm.c

noinst_HEADERS= \
        MwmUtil.h \
	applications.h \
	handlers.h \
	init.h \
	menu.h \
	move.h\
	nodes.h \
	placement.h \
	properties.h \
	resize.h \
	rubber.h \
	special.h \
	uwm.h \
	uwmmenu.h \
	widgets.h \
	windows.h \
	winmenu.h \
	urm.h \
	winmenumenu.h \
	pix.h \
	selection.h \
	wingroups.h \
	tree.h \
	confparse.h \
	settings.h \
	confparse.h \
	confparse.tab.h \
	workspaces.h

EXTRA_DIST= shape.bitmap confparse.l confparse.y 

# no, folks - this is not for public use so configure doesn't check for
# flex/yacc. people who compile from cvs should know what to do.

# YACC = yacc # bison -y will not work
FLEX = flex

confparse.tab.c: confparse.y
	@(which $(YACC) >/dev/null \
          && test -z "`$(YACC) --version |grep -i bison`") \
          || (echo "	====================================================" ;\
              echo "	======> NEEDS BERKLEY YACC TO BUILD FROM CVS <======" ;\
              echo "	======>  BISON DOES NOT WORK FOR THE MOMENT  <======" ;\
              echo "	====================================================" ;\
              exit 1)
	$(YACC) -d -b confparse confparse.y

confparse.tab.h: confparse.tab.c

confparse.lex.c: confparse.l confparse.tab.h
	@which $(FLEX) >/dev/null \
          || (echo "	==================================================" ;\
              echo "	======>   INSTALL FLEX TO BUILD FROM CVS   <======" ;\
              echo "	==================================================" ;\
              exit 1)
	$(FLEX) -oconfparse.lex.c confparse.l

cleanparser: clean
	rm -f *.tab.c *.lex.c *.tab.h
