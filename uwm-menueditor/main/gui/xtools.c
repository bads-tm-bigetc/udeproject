#include "X11/Xlib.h"
#include "X11/Xutil.h"
#include "X11/Xos.h"
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "xtools.h"

Display* dis;
XContext xctxt;
int screen;
XrmDatabase xdb;
XFontStruct* font;
Bool mainquit;
Atom wmDeleteMessage;

XObject* modalwindow;

Bool paste_requested;
Window selowner;
XObject* pasteexp;
int pastepos;

Bool sel_type;
char* sel_text;

Bool responded;
int response;

char buff[256];
void x_init()
{
	xctxt=XUniqueContext();
	dis=XOpenDisplay(NULL);
	if(!dis)
	{
		fprintf(stderr,"Failed to connect to display\n");
		exit(2);
	}
	screen=DefaultScreen(dis);
	font=XLoadQueryFont(dis,"fixed");
	wmDeleteMessage = XInternAtom(dis, "WM_DELETE_WINDOW", False);
	paste_requested=False;
	modalwindow=NULL;
	responded=False;
	response=0;
}

void x_clear_modal()
{
	modalwindow=NULL;
}

void x_main()
{
	XEvent event;
	mainquit=False;
	while(!mainquit)
	{
		XNextEvent(dis,&event);
		x_handle_event(event);
	}
}

void x_handle_event(XEvent event)
{
	XObject *obj;
	if(event.type==SelectionNotify)
	{
		if(paste_requested)
		{
			if(event.xselection.property!=None)
				switch(pasteexp->obj.type)
				{
				case XWINDOW:
				case XBUTTON:
				case XFRAME:
				case XLABEL:
					break;
				case XTEXTBOX:
					x_textbox_handle_event(pasteexp,event);
					break;
				}
		}
	}
	else if(event.type==SelectionRequest)
	{
		XEvent resp;
		XSelectionRequestEvent* req=&(event.xselectionrequest);
		if(req->target == sel_type && sel_type==XA_STRING)
		{
			XChangeProperty(dis,req->requestor,req->property,XA_STRING,8,PropModeReplace,(unsigned char*)sel_text,strlen(sel_text));
			resp.xselection.property=req->property;
		}
		else
			resp.xselection.property=None;
		resp.xselection.type=SelectionNotify;
		resp.xselection.display=req->display;
		resp.xselection.requestor=req->requestor;
		resp.xselection.selection=req->selection;
		resp.xselection.target= req->target;
		resp.xselection.time = req->time;
		XSendEvent(dis,req->requestor,0,0,&resp);
	}
	else
	{
		obj=NULL;
		XFindContext(event.xany.display, event.xany.window, xctxt, (XPointer*)&obj);
		if(obj!=NULL)
		{
			if(obj->obj.parentwindow->window.blocked&&event.type!=Expose)
				return;
			if((obj->obj.parentwindow!=modalwindow&&modalwindow!=NULL)&&event.type!=Expose)
				return;
			if(event.type==ButtonPress)
				if(!obj->obj.isfocused)
					x_object_set_focused(obj);
			if(event.type==KeyPress)
			{
				XObject* cur=obj->obj.parentwindow->window.focus;
				switch(cur->obj.type)
				{
				case XWINDOW:
					break;
				case XBUTTON:
					break;
				case XFRAME:
					break;
				case XLABEL:
					break;
				case XTEXTBOX:
					x_textbox_handle_event(cur,event);
					break;
				}
			}
			else
			{
				switch(obj->obj.type)
				{
				case XWINDOW:
					x_window_handle_event(obj,event);
					break;
				case XBUTTON:
					x_button_handle_event(obj,event);
					break;
				case XFRAME:
					x_frame_handle_event(obj,event);
					break;
				case XLABEL:
					x_label_handle_event(obj,event);
					break;
				case XTEXTBOX:
					x_textbox_handle_event(obj,event);
					break;
				}
			}
		}
	}
}


void x_main_quit()
{
	mainquit=True;
}

