#ifndef _CONFIG_H
#define _CONFIG_H

/*** Configurable macros ***/

/***************************/

@TOP@

/* we seem to need this */
#undef PACKAGE
#undef VERSION

/* define this if you don't want uwm to use your x-servers BackingStore *
 * and SaveUnder features (which are buggy in XF4).                     */
#undef DISABLE_BACKING_STORE

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
