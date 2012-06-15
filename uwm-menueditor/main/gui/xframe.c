#include "xframe.h"
#include <stdio.h>
#include <stdlib.h>

extern Display* dis;
extern XContext xctxt;
extern int screen;
extern XrmDatabase xdb;
extern XFontStruct* font;
extern Atom wmDeleteMessage;

XObject* x_frame_create(XObject* parent, int x, int y, unsigned width, unsigned height,
		Bool visible, unsigned bordersize, unsigned long bordercolor, unsigned long background,
		XPointer cbdata)
{
	XObject* f=(XObject*) calloc(1,sizeof(XObject));
	if(!f)
	{
		fprintf(stderr,"Impossible to create window: out of memory!\n");
		exit(OUT_OF_MEMORY);
	}
	f->frame.obj.type=XFRAME;
	f->frame.obj.cbdata=cbdata;
	f->frame.obj.dis=dis;
	f->obj.height=height;
	f->obj.width=width;
	f->frame.x=x;
	f->frame.y=y;
	f->frame.visible=visible;
	f->frame.obj.background=background;
	f->frame.obj.win=XCreateSimpleWindow(dis,parent->obj.win,x,y,width,height,bordersize,bordercolor,background);
	if(!f->frame.obj.win)
	{
		fprintf(stderr,"Failed to create window\n");
		exit(2);
	}
	f->obj.parentwindow=parent->obj.parentwindow;
	f->obj.isfocused=False;
	XSelectInput(dis,f->obj.win,KeyPressMask|ExposureMask);
	f->frame.visible=visible;
	if(f->frame.visible)
	{
		XMapRaised(dis,f->frame.obj.win);
	}

	XSaveContext(dis,f->frame.obj.win,xctxt,(char*)f);
	return f;
}

void x_frame_set_visible(XObject* f, Bool visible)
{
	if(f->obj.type==XFRAME)
	{
		if(f->frame.visible!=visible)
		{
			f->frame.visible=visible;
			if(f->frame.visible)
				XMapWindow(dis,f->frame.obj.win);
			else
				XUnmapWindow(dis,f->frame.obj.win);
		}
	}
	else
		fprintf(stderr,"Warning: x_frame_set_visible: Wrong type of first argument\n");
}

void x_frame_set_expose(XObject* fr, Callback f)
{
	if(fr->obj.type==XFRAME)
	{
		fr->frame.expose=f;
	}
	else
		fprintf(stderr,"Warning: x_frame_set_expose: Wrong type of first argument\n");
}

void x_frame_handle_event(XObject* obj, XEvent event)
{
	if(obj->frame.visible)
		switch(event.type)
		{
		case Expose:
			x_object_exposed(obj,event);
			break;
		}
}
