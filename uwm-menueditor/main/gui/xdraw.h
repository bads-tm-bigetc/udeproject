#ifndef XDRAW_H_
#include "xobject.h"

void x_draw_point(XObject* obj, GC gc,int x, int y);
void x_draw_arc(XObject* obj,GC gc,int lx,int ly,unsigned width, unsigned height,int start_angle,int end_angle);
void x_draw_rectangle(XObject* obj, GC gc, int lx,int ly, unsigned width, unsigned height);
void x_draw_circle(XObject* obj,GC gc, int cx,int cy, unsigned rad);
void x_fill_arc(XObject* obj,GC gc,int lx,int ly,unsigned width, unsigned height,int start_angle,int end_angle);
void x_fill_rectangle(XObject* obj, GC gc, int lx,int ly, unsigned width, unsigned height);
void x_fill_circle(XObject* obj,GC gc, int cx,int cy, unsigned rad);

#endif
