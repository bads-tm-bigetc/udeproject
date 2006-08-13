/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifndef HANDLERS_H
#define HANDLERS_H

#include <X11/Xlib.h>

/*** Prototypes ***/

void UWMErrorHandler(Display *disp, XErrorEvent *Err);
void ArmeageddonHandler(void);
void RedirectErrorHandler(void);
void InitHandlers();
void TermHandler(int dummy);
void ReinstallDefaultHandle();
void HandleShape(XEvent *event);
void InstallMoveHandle();
void InstallResizeHandle();
void InstallWinMenuHandle();
void InstallMenuHandle();
void HandleUnmapNotify(XEvent *event);
void HandleExpose(XEvent *event);

#endif
