/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifndef UWM_MOVE_H
#define UWM_MOVE_H

void MoveButtonPress(XEvent *event);
void MoveMotion(XEvent *event);
void MoveButtonRelease(XEvent *event);
void MoveUnmap(XEvent *event);
void StartDragging(UltimateContext *uc,unsigned int x,unsigned int y);

#endif
