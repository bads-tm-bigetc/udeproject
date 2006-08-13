/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifndef UWM_PLACEMENT_H
#define UWM_PLACEMENT_H

typedef struct {
  int x1,y1,x2,y2;
} ScanData;

void PlaceWin(UltimateContext *uc);
NodeList *ScanScreen(UltimateContext *win);
void FreeScanned(NodeList *wins);
void SnapWin(NodeList *wins, int *x, int *y, int width, int height);

#endif
