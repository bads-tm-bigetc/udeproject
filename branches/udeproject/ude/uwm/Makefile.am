bin_PROGRAMS= uwm
# When the library be ready
# uwm_LDADD= ../lib/libude.la
uwm_LDFLAGS= ${X_LIBS} ${X_EXTRA_LIBS} ${X_PRE_LIBS}
# I think this should be diferent from the ude version.
INCLUDES= ${X_CFLAGS} -I../lib -DUWMVERSION=\"@VERSION@\" \
	-DLOCALEDIR=\"${datadir}/locale\" -DUDE_DIR=\"${pkgdatadir}\"
uwm_SOURCES=  \
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
	workspaces.h

EXTRA_DIST= shape.bitmap
