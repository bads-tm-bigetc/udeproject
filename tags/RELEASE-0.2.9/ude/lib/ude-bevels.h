#ifndef UDE_BEVELS_H
#define UDE_BEVELS_H

#include "ude-box.h"

void
ude_draw_bevel (UDEBox *box, int x1, int y1, int x2, int y2, int width,
                GC NW, GC SE);
void
ude_draw_groove (UDEBox *box, int x1, int y1, int x2, int y2, int width,
                 GC NW, GC SE);

#endif
