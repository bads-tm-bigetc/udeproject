/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifndef UDE_WIDGETS_H
#define UDE_WIDGETS_H

void DrawBevel(Drawable win,int x1,int y1,int x2,int y2,int width,GC NW,GC SE);
void DrawFrameBevel(UltimateContext *uc);
void DrawTitle(UltimateContext *uc);

#endif
