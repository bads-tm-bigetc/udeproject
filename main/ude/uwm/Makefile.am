BUILT_SOURCES= confparse.tab.h

AM_YFLAGS= -d

bin_PROGRAMS= uwm

# When the library be ready
# uwm_LDADD= ../lib/libude.la

uwm_LDFLAGS= ${X_LIBS} ${X_EXTRA_LIBS} ${X_PRE_LIBS}

# I think this should be diferent from the ude version.
INCLUDES= ${X_CFLAGS} -I../lib -DUWMVERSION=\"@VERSION@\" \
	-DLOCALEDIR=\"${datadir}/locale\" -DUDE_DIR=\"${pkgdatadir}\" \
	-DUDE_BIN_PATH=\"${bindir}\"

uwm_SOURCES=  \
    confparse.lex.l \
    confparse.tab.y \
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
	ude_config_consts.h \
	uwm_intl.h \
	workspaces.h

EXTRA_DIST= shape.bitmap
