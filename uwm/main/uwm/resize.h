/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifndef UWM_RESIZE_H
#define UWM_RESIZE_H

void StartResizing(UltimateContext *uc,int x,int y);
void ResizeButtonPress(XEvent *event);
void ResizeMotion(XEvent *event);
void ResizeButtonRelease(XEvent *event);

#endif
