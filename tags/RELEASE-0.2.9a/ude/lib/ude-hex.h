#ifndef UDE_HEX_H
#define UDE_HEX_H

#include "ude-desktop.h"
#include "ude-box.h"

/* will calculate the measurements of a rectangle surrounding a Hex with respect
   to the given mode. The returned width and height values can directly be
   passed to ude_create_hex_shape. However bevel widthes will have to be added
   manually if needed. */

#define UDEHexExactHeight (0)
#define UDEHexFitInside (1)
#define UDEHexFitOutside (2)
#define UDEHexEdgeLenght (3)       /* width = edge length, height ignored. */

void
ude_calc_hex_measures(int *width, int *height, unsigned char mode);

/* returns a 1-plane pixmap that can be used as shape for a hex fitting exactly
   into a width x height rectangle. returns None if pixmap creation fails.
   the widht and height of the pixmap are width and height. */

Pixmap
ude_create_hex_shape(UDEDesktop *desk, short width, short height);

/* draws a hex bevel. NE and SW are the GCs to be used for northeastern and
   southwestern bevels. coordinates are the same as in ude_create_hex_shape */

void
ude_draw_hex_bevel (UDEBox *box, int x1, int y1, int x2, int y2, int width,
                    GC NW, GC SE);

/* draws a hex groove. NE and SW are the GCs to be used for northeastern and
   southwestern bevels. coordinates are the same as in ude_create_hex_shape */

void
ude_draw_hex_groove (UDEBox *box, int x1, int y1, int x2, int y2, int width,
                     GC NW, GC SE);

/* will calculate the coordinates for a hex with width w2 next to a hex with
   width w1 which is positioned at x,y. both hexes must have the same height h.
   data is returned in x and y. */
   
#define UDEHexLeft   (0)
#define UDEHexCenter (1)
#define UDEHexRight  (2)

#define UDEHexAbove  (0)
#define UDEHexBelow  (1)

void
ude_hex_coords(int w1, int w2, int h, int *x, int *y, 
                char hpos, char vpos);

#endif
