#include "xtools.h"

void x_draw_point(XObject* obj, GC gc,int x, int y)
{
	XDrawPoint(obj->obj.dis,obj->obj.win,gc,x,y);
}

void x_draw_arc(XObject* obj,GC gc,int lx,int ly,unsigned width, unsigned height,int start_angle,int end_angle)
{
	XDrawArc(obj->obj.dis,obj->obj.win,gc,lx,ly,width,height,start_angle,end_angle);
}

void x_draw_circle(XObject* obj,GC gc, int cx,int cy, unsigned rad)
{
	x_draw_arc(obj,gc,cx-rad,cy-rad,2*rad,2*rad,0,360*64);
}

void x_draw_rectangle(XObject* obj, GC gc, int lx,int ly, unsigned width, unsigned height)
{
	XDrawRectangle(obj->obj.dis,obj->obj.win,gc,lx,ly,width,height);
}

void x_fill_arc(XObject* obj,GC gc,int lx,int ly,unsigned width, unsigned height,int start_angle,int end_angle)
{
	XFillArc(obj->obj.dis,obj->obj.win,gc,lx,ly,width,height,start_angle,end_angle);
}

void x_fill_circle(XObject* obj,GC gc, int cx,int cy, unsigned rad)
{
	x_fill_arc(obj,gc,cx-rad,cy-rad,2*rad,2*rad,0,360*64);
}

void x_fill_rectangle(XObject* obj, GC gc, int lx,int ly, unsigned width, unsigned height)
{
	XFillRectangle(obj->obj.dis,obj->obj.win,gc,lx,ly,width,height);
}
