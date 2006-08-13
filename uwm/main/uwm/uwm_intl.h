/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifndef _UDE_I18N_H
#define _UDE_I18N_H

#undef _
#undef N_

#ifdef ENABLE_NLS

#include <libintl.h>

#define _(String) gettext (String)

#ifdef gettext_noop
#define N_(String) gettext_noop (String)
#else /* !gettext_noop */
#define N_(String) (String)
#endif /* !gettext_noop */

#else /* !ENABLE_NLS */

#define _(String) (String)
#define N_(String) (String)

#endif /* !ENABLE_NLS */

#endif /* !_UDE_I18N_H */
