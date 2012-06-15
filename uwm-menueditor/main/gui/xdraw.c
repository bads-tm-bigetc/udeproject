#include "xtools.h"

void x_draw_point(XObject* obj, XToolsGC gc,int x, int y)
{
	XDrawPoint(obj->obj.dis,obj->obj.win,gc.dat.gc,x,y);
}

void x_draw_arc(XObject* obj, XToolsGC gc,int lx,int ly,unsigned width, unsigned height,int start_angle,int end_angle)
{
	XDrawArc(obj->obj.dis,obj->obj.win,gc.dat.gc,lx,ly,width,height,start_angle,end_angle);
}

void x_draw_circle(XObject* obj, XToolsGC gc, int cx,int cy, unsigned rad)
{
	x_draw_arc(obj,gc,cx-rad,cy-rad,2*rad,2*rad,0,360*64);
}

void x_draw_rectangle(XObject* obj, XToolsGC gc, int lx,int ly, unsigned width, unsigned height)
{
	XDrawRectangle(obj->obj.dis,obj->obj.win,gc.dat.gc,lx,ly,width,height);
}

void x_fill_arc(XObject* obj, XToolsGC gc,int lx,int ly,unsigned width, unsigned height,int start_angle,int end_angle)
{
	XFillArc(obj->obj.dis,obj->obj.win,gc.dat.gc,lx,ly,width,height,start_angle,end_angle);
}

void x_fill_circle(XObject* obj, XToolsGC gc, int cx,int cy, unsigned rad)
{
	x_fill_arc(obj,gc,cx-rad,cy-rad,2*rad,2*rad,0,360*64);
}

void x_fill_rectangle(XObject* obj, XToolsGC gc, int lx,int ly, unsigned width, unsigned height)
{
	XFillRectangle(obj->obj.dis,obj->obj.win,gc.dat.gc,lx,ly,width,height);
}

void x_draw_line( XObject* obj, XToolsGC gc, int x1,int y1,int x2,int y2)
{
    XDrawLine(obj->obj.dis,obj->obj.win,gc.dat.gc,x1,y1,x2,y2);
}

void x_pixmap_copy( XObject* dst, XObject* src, XToolsGC gc, int srcx, int srcy, unsigned width, unsigned height, int dstx, int dsty)
{
	XCopyArea(dst->obj.dis, src->obj.win, dst->obj.win, gc.dat.gc, srcx, srcy, width, height, dstx, dsty);
}

