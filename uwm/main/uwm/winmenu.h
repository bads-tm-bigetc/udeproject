/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifndef UWM_WINMENU_H
#define UWM_WINMENU_H

void StartWinMenu(UltimateContext *uc,int x,int y);
void WinMenuEnterNotify(XEvent *event);
void WinMenuButtonRelease(XEvent *event);
void WinMenuButtonPress(XEvent *event);
void WinMenuVisibility(XEvent *event);
void WinMenuUnmapNotify(XEvent *event);

#endif
