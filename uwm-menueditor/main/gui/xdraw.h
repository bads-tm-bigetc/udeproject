#ifndef XDRAW_H_
#include "xobject.h"

void x_draw_point(XObject* obj, XToolsGC gc,int x, int y);
void x_draw_arc(XObject* obj, XToolsGC gc,int lx,int ly,unsigned width, unsigned height,int start_angle,int end_angle);
void x_draw_rectangle(XObject* obj, XToolsGC gc, int lx,int ly, unsigned width, unsigned height);
void x_draw_circle(XObject* obj, XToolsGC gc, int cx,int cy, unsigned rad);
void x_fill_arc(XObject* obj, XToolsGC gc,int lx,int ly,unsigned width, unsigned height,int start_angle,int end_angle);
void x_fill_rectangle(XObject* obj, XToolsGC gc, int lx,int ly, unsigned width, unsigned height);
void x_fill_circle(XObject* obj, XToolsGC gc, int cx,int cy, unsigned rad);
void x_draw_line( XObject* obj, XToolsGC gc, int x1,int y1,int x2,int y2);
void x_pixmap_copy( XObject* dst, XObject* src, XToolsGC gc, int srcx, int srcy, unsigned width, unsigned height, int dstx, int dsty);

#endif
