
#include "X11/Xlib.h"
#include "ude-desktop.h"
#include "ude-hex.h"

/* will calculate the measurements of a rectangle surrounding a Hex with respect
   to the given mode. The returned width and height values can directly be
   passed to ude_create_hex_shape. However bevel widthes will have to be added
   manually if needed. */
  
void
ude_calc_hex_measures(int *width, int *height, unsigned char mode)
{
  switch(mode) {
    case UDEHexExactHeight:
      *width += *height * 0.57735027;          //  height * tan(30 deg)
      break;
    case UDEHexFitInside:
      *width += *height * 0.57735027;          //  height * tan(30 deg)
      *height *= 1.4;
      break;
    case UDEHexFitOutside:
       // do nothing; these are exactly the values having to be returned.
      break;
    case UDEHexEdgeLenght: // in this case with = edge length; height irrelevant
      *width *= 2;
      *height = *width * 0.8660254;            //  width * sin(60 deg)
      break;
    default:
      *width = *height = -1;                   //  error
      break;
  }
}

/* returns a 1-plane pixmap that can be used as shape for a hex fitting exactly
   into a width x height rectangle. */

Pixmap
ude_create_hex_shape(UDEDesktop *desk, short width, short height)
{
  Pixmap pm;
  GC gc;
  XGCValues xgcv;
  XPoint points[8];

  if((width <= 0) || (height <= 0)) return None;

  pm = XCreatePixmap(desk->disp,
                     RootWindow (desk->disp, DefaultScreen(desk->disp)),
                     width, height, 1);
  if(None == pm) return None;

  xgcv.function = GXclear;
  gc = XCreateGC(desk->disp, pm, GCFunction, &xgcv);

  XFillRectangle(desk->disp, pm, gc, 0, 0, width, height);

  xgcv.function = GXset;
  XChangeGC(desk->disp, gc, GCFunction, &xgcv);

  points[0].x = points[7].x = 0;
  points[3].x = points[4].x = width;
  points[0].y = points[3].y = height >> 1;
  points[7].y = points[4].y = height - points[0].y;
  points[1].y = points[2].y = 0;
  points[5].y = points[6].y = height;
  points[1].x = points[6].x = height * 0.2886751346 + 0.5;
  points[2].x = points[5].x = width - points[1].x;
  XFillPolygon(desk->disp, pm, gc, points, 8, Convex, CoordModeOrigin);

  XFreeGC(desk->disp, gc);
  return pm;
}

void
ude_draw_hex_bevel (UDEBox *box, int x1, int y1, int x2, int y2, int width,
                    GC NW, GC SE)
{
  XPoint points[8], offspoints[8];
  short a, w, h;

  w = x2 - x1;
  h = y2 - y1;
  points[0].x = 0;
  points[5].x = width;
  points[0].y = points[5].y = h >> 1;
  points[1].y = points[2].y = 0;
  points[7].y = h;
  points[6].y = h - width;
  points[4].y = points[3].y = width;
  points[1].x = points[7].x = h * 0.2886751346 + 0.5;
  points[2].x = w - points[1].x;
  points[4].x = points[6].x = (h + width) * 0.2886751346 + 0.5;
  points[3].x = w - points[4].x;

  for(a = 0; a < 8; a++) {
    offspoints[a].x = x1 + points[a].x;
    offspoints[a].y = y1 + points[a].y;
  }
  XFillPolygon(box->desktop->disp, box->win, NW, offspoints, 8, Nonconvex,
               CoordModeOrigin);

  for(a = 0; a < 8; a++) {
    offspoints[a].x = x2 - points[a].x;
    offspoints[a].y = y2 - points[a].y;
  }
  XFillPolygon(box->desktop->disp, box->win, SE, offspoints, 8, Nonconvex,
               CoordModeOrigin);
}

void
ude_draw_hex_groove (UDEBox *box, int x1, int y1, int x2, int y2, int width,
                     GC NW, GC SE)
{
  ude_draw_hex_bevel (box, x1, y1, x2, y2, width/2, NW, SE);
  ude_draw_hex_bevel (box, x1 + width/2 , y1 + width/2,
                      x2 - width/2, y2 - width/2, width - width/2, SE, NW);
}

void
ude_hex_coords(int w1, int w2, int h, int *x, int *y,
                char hpos, char vpos)
{
  if(hpos == UDEHexCenter) {
    if(vpos == UDEHexAbove) *y -= 2 * (h - (h >> 1));
    else *y += 2 * (h - (h >> 1));
  } else {
    if(hpos == UDEHexLeft) *x -= w2 - h * 0.2886751346 - 0.5;
    else *x += w1 - h * 0.2886751346 + 0.5;

    if(vpos == UDEHexAbove) *y -= h >> 1;
    else *y += h - (h >> 1);
  }
}
