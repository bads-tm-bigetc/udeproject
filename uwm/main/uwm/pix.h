/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifndef UWM_PIX_H
#define UWM_PIX_H

#include <X11/Xlib.h>
#include <X11/xpm.h>

int LoadPic(char *filename, Pixmap *pm, XpmAttributes *xa);
void FreePic(Pixmap *pm, XpmAttributes **xa);

#endif
