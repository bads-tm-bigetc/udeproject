#include "xtools.h"
#include <stdio.h>
#include <stdlib.h>

extern Display* dis;
extern XContext xctxt;
extern int screen;
extern XrmDatabase xdb;
extern XFontStruct* font;
extern Bool mainquit;
extern Atom wmDeleteMessage;

extern XObject* modalwindow;

extern Bool responded;
extern int response;

int x_dialog_run(XObject* obj)
{
	if(obj->obj.type!=XWINDOW)
	{
		fprintf(stderr,"Warning: x_dialog_run: Wrong type of first argument\n");
		return -1;
	}
	Bool prev_responded=responded;
	int prev_response=response;
	int res;
	int x,y;
	Window root,par,*childlist,dummy;
	unsigned childcnt;
	XObject* prevmodal=modalwindow;
	XWindowAttributes attr;
	XEvent event;
	responded=False;
	response=-1;
	XObject* parent=x_window_get_transient(obj);
	if(parent!=NULL)
	{
		if(parent->obj.type!=XWINDOW)
			fprintf(stderr,"Warning: x_dialog_run: Wrong type of second argument\n");
		else
		{
			XGetWindowAttributes(dis,parent->obj.win,&attr);
			if(XQueryTree(dis,parent->obj.win,&root,&par,&childlist,&childcnt))
			{
				XTranslateCoordinates(dis,par,root,attr.x,attr.y,&x,&y,&dummy);
				x += parent->obj.width / 2 - obj->obj.width / 2;
				y += parent->obj.height / 2 - obj->obj.height / 2;
				XMoveWindow(dis,obj->obj.win,x,y);
				if(childlist)
					XFree(childlist);
			}
			else
				fprintf(stderr,"Warning: x_dialog_run: Failed to position window");
		}
	}
	x_window_set_visible(obj,True);
	x_window_set_modal(obj,True);
	while(!responded&&!mainquit)
	{
		XNextEvent(dis,&event);
		x_handle_event(event);
	}
	res=response;
	responded=prev_responded;
	response=prev_response;
	modalwindow=prevmodal;
	x_window_set_visible(obj,False);
	return res;
}

void x_dialog_send_response(int resp)
{
	responded=True;
	response=resp;
}
