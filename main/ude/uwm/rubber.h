/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifndef UWM_RUBBER_H
#define UWM_RUBBER_H

void StartRubber(unsigned int lx,unsigned int ly,unsigned int lwidth,\
                       unsigned int lheight,unsigned int BorderWidth);
void SqueezeRubber(unsigned int lx,unsigned int ly,unsigned int lwidth,\
                                                  unsigned int lheight);
void StopRubber(unsigned int *lx,unsigned int *ly,unsigned int *lwidth,\
                                                 unsigned int *lheight);

#endif
