#ifndef _CONFIG_H
#define _CONFIG_H

/*** Configurable macros ***/

/***************************/

@TOP@

/* we seem to need this */
#undef PACKAGE
#undef VERSION

/* command line for preprocessor */
#undef CPP_CALL

/* For i18n */
#undef ENABLE_NLS
#undef HAVE_CATGETS
#undef HAVE_GETTEXT
#undef HAVE_LC_MESSAGES
#undef HAVE_STPCPY

@BOTTOM@

#endif /* !_CONFIG_H */
