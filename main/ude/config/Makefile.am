DIR_CONTENTS= \
        StartScript \
	StopScript \
	appmenu \
	mountmenu \
	urdb \
	uwmrc \
	uwmrc-behaviour.hook \
	uwmrc-layout.hook \
	uwmrc-ws.hook
	
udeconfigdir= ${pkgdatadir}/config

udeconfig_DATA= \
	appmenu \
	mountmenu \
	urdb \
	uwmrc \
	uwmrc-behaviour.hook \
	uwmrc-layout.hook \
	uwmrc-ws.hook \
	ude_config_consts.h

dist-hook:
	ln -s ../uwm/ude_config_consts.h $(distdir)

udeconfig_SCRIPTS= \
	StartScript \
	StopScript

EXTRA_DIST= ${DIR_CONTENTS}

