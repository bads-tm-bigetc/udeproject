#include "xobject.h"
#include "xwindow.h"
#include <stdio.h>
#include <stdlib.h>

extern Display* dis;
extern XContext xctxt;
extern int screen;
extern XrmDatabase xdb;
extern XFontStruct* font;
extern Atom wmDeleteMessage;
extern XObject* modalwindow;
XObject* x_window_create(unsigned width,unsigned height,
		Bool visible,unsigned bordersize, unsigned long int bordercolor,
		unsigned long int background, char* title, XPointer cbdata)
{
	XObject* w=(XObject*) calloc(1,sizeof(XObject));
	if(!w)
	{
		fprintf(stderr,"Impossible to create window: out of memory!\n");
		exit(OUT_OF_MEMORY);
	}
	w->window.obj.type=XWINDOW;
	w->window.obj.dis=dis;
	w->obj.height=height;
	w->obj.width=width;
	w->window.visible=visible;
	w->window.blocked=False;
	w->window.obj.background=background;
	w->window.obj.win=XCreateSimpleWindow(dis,DefaultRootWindow(dis),100,0,width,height,bordersize,bordercolor,background);
	if(!w->window.obj.win)
	{
		fprintf(stderr,"Failed to create window\n");
		exit(2);
	}
	if(!XStringListToTextProperty(&title,1,&w->window.header))
	{
		fprintf(stderr,"Impossible to create window: out of memory!\n");
		exit(OUT_OF_MEMORY);
	}
	XWMHints *xwmhHints=XAllocWMHints();
	if(!xwmhHints)
	{
		fprintf(stderr,"Impossible to create window: out of memory!\n");
		exit(OUT_OF_MEMORY);
	}
	xwmhHints->initial_state=NormalState;
	xwmhHints->input=True;
	xwmhHints->flags=InputHint|StateHint;
	XSetWMProperties(dis,w->obj.win,&w->window.header,None,None,0,None,xwmhHints,None);
	XFree(xwmhHints);
	w->window.obj.parentwindow=w;
	w->window.focus=w;
	w->obj.isfocused=True;
	w->obj.cbdata=cbdata;
	w->window.visible=visible;
	w->window.expose=NULL;
	w->window.button_press=NULL;
	w->window.configure=NULL;
	w->window.destroy=NULL;
	XSelectInput(dis, w->window.obj.win, ButtonPressMask|ButtonReleaseMask|ExposureMask|
			FocusChangeMask|KeyPressMask|StructureNotifyMask);
	XSetWMProtocols(dis,w->obj.win,&wmDeleteMessage,1);
	if(w->window.visible)
	{
		XMapRaised(dis,w->window.obj.win);
	}
	XSaveContext(dis,w->window.obj.win,xctxt,(char*)w);
	return w;
}

XObject* x_window_create_transient(XObject* parent, unsigned width,unsigned height,
		Bool visible,unsigned bordersize, unsigned long int bordercolor,
		unsigned long int background, char* title, XPointer cbdata)
{
	XObject* win=x_window_create(width,height,False,bordersize,bordercolor,background,title,cbdata);
	XWindowAttributes attr;
	Window root,par,*childlist,dummy;
	unsigned childcnt;
	int x,y;
	XSetTransientForHint(dis,win->obj.win,parent->obj.win);
	XSizeHints *xszh=XAllocSizeHints();
	if(!xszh)
	{
		fprintf(stderr,"Error: x_window_create_transient: Out of memory");
		exit(OUT_OF_MEMORY);
	}
	xszh->flags=USPosition;
	XSetWMNormalHints(dis,win->obj.win,xszh);

	XGetWindowAttributes(dis,parent->obj.win,&attr);
	if(XQueryTree(dis,parent->obj.win,&root,&par,&childlist,&childcnt))
	{
		XTranslateCoordinates(dis,par,root,attr.x,attr.y,&x,&y,&dummy);
		x += parent->obj.width / 2 - win->obj.width / 2;
		y += parent->obj.height / 2 - win->obj.height / 2;
		XMoveWindow(dis,win->obj.win,x,y);
		if(childlist)
			XFree(childlist);
	}
	else
		fprintf(stderr,"x_window_create_transient: Failed to position window");
	x_window_set_visible(win,visible);

	return win;
}

XObject* x_window_get_transient(XObject* w)
{
	Window rw;
	XObject* res=NULL;
	if(!XGetTransientForHint(dis,w->obj.win,&rw))
		return NULL;
	XFindContext(dis,rw,xctxt,(XPointer*)&res);
	return res;
}

void x_window_set_visible(XObject* w, Bool visible)
{
	if(w->obj.type==XWINDOW)
	{
		if(w->window.visible!=visible)
		{
			w->window.visible=visible;
			if(w->window.visible)
				XMapRaised(dis,w->window.obj.win);
			else
				XUnmapWindow(dis,w->window.obj.win);
		}
	}
	else
		fprintf(stderr,"Warning: x_window_set_visible: Wrong type of first argument\n");
}

void x_window_set_expose(XObject* w, Callback f)
{
	if(w->obj.type==XWINDOW)
	{
		w->window.expose=f;
	}
	else
		fprintf(stderr,"Warning: x_window_set_expose: Wrong type of first argument\n");
}

void x_window_set_button_press(XObject* w, Callback f)
{
	if(w->obj.type==XWINDOW)
	{
		w->window.button_press=f;
	}
	else
		fprintf(stderr,"Warning: x_window_set_button_press: Wrong type of first argument\n");
}

void x_window_set_destroy_event(XObject* w, Callback f)
{
	if(w->obj.type==XWINDOW)
	{
		w->window.destroy=f;
	}
	else
		fprintf(stderr,"Warning: x_window_set_button_press: Wrong type of first argument\n");
}

void x_window_set_configure(XObject* w, Callback f)
{
	if(w->obj.type==XWINDOW)
	{
		w->window.configure=f;
	}
	else
		fprintf(stderr,"Warning: x_window_set_configure: Wrong type of first argument\n");
}

void x_window_set_blocked(XObject* w, Bool blocked)
{
	if(w->obj.type==XWINDOW)
	{
		w->window.blocked=blocked;
	}
	else
		fprintf(stderr,"Warning: x_window_set_blocked: Wrong type of first argument\n");
}

void x_window_set_modal(XObject* w,Bool modal)
{
	if(w->obj.type==XWINDOW)
	{
		if(modal)
			modalwindow=w;
		else if(w==modalwindow)
			modalwindow=NULL;
	}
	else
		fprintf(stderr,"Warning: x_window_set_modal: Wrong type of first argument\n");
}

void x_window_handle_event(XObject* obj, XEvent event)
{
	if(obj->window.visible)
		switch(event.type)
		{
		case Expose:
			x_object_exposed(obj,event);
			break;
		case ButtonPress:
			if(obj->window.button_press)
				obj->window.button_press(obj,event,obj->obj.cbdata);
			break;
		case ClientMessage:
			if(event.xclient.data.l[0]==wmDeleteMessage)
				if(!obj->window.destroy||!obj->window.destroy(obj,event,obj->obj.cbdata))
					x_object_destroy(obj);
			break;
		case ConfigureNotify:
			obj->obj.height=event.xconfigure.height;
			obj->obj.width=event.xconfigure.width;
			if(obj->window.configure)
				obj->window.configure(obj,event,obj->obj.cbdata);
			break;
		}
}
