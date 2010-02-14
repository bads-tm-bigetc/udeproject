
#include "ude-bevels.h"

void
ude_draw_bevel (UDEBox *box, int x1, int y1, int x2, int y2, int width,
                GC NW, GC SE)
{
  int a;
  for(a=0;a<width;a++){
    XDrawLine (box->desktop->disp, box->win, NW, x1+a, y1+a, x2-a-1, y1+a);
    XDrawLine (box->desktop->disp, box->win, NW, x1+a, y1+a+1, x1+a, y2-a);
    XDrawLine (box->desktop->disp, box->win, SE, x1+a+1, y2-a, x2-1-a, y2-a);
    XDrawLine (box->desktop->disp, box->win, SE, x2-a, y1+a, x2-a, y2-a);
  }
}

void
ude_draw_groove (UDEBox *box, int x1, int y1, int x2, int y2, int width,
                 GC NW, GC SE)
{
  ude_draw_bevel (box, x1, y1, x2, y2, width/2, NW, SE);
  ude_draw_bevel (box, x1 + width/2 , y1 + width/2,
                  x2 - width/2, y2 - width/2, width - width/2, SE, NW);
}
