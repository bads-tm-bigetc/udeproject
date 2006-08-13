/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifndef UWM_SELECTION_H
#define UWM_SELECTION_H

#include <X11/Xlib.h>
#include <X11/Xatom.h>

void SendSelectionNotify(XEvent *request, Atom property);
int ChangeProperty(Window win, Atom property, Atom type, int format, int mode, 
                   char *data, int nelements);
Atom Parse_WM_Sx_Selection(Atom target, Atom prop, Window requestor);

#endif
