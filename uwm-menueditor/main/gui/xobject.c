#include "xobject.h"
#include "xwindow.h"
#include "xbutton.h"
#include "xframe.h"
#include "xlabel.h"
#include "xtextbox.h"
#include <stdio.h>
#include <stdlib.h>
extern Display* dis;
extern XContext xctxt;
extern int screen;
extern XrmDatabase xdb;
extern XFontStruct* font;
extern Atom wmDeleteMessage;
extern XObject* modalwindow;

void x_object_destroy(XObject * obj)
{
	Window root,parent,*childlist;
	XObject* child;
	unsigned childcnt,i;
	if(XQueryTree(dis,obj->obj.win,&root,&parent,&childlist,&childcnt)!=0)
	{
		if(childlist!=NULL)
		{
			for(i=0;i<childcnt;i++)
			{
				child=NULL;
				XFindContext(dis,childlist[i],xctxt,(XPointer*)&child);
				if(child!=NULL)
					x_object_destroy(child);
			}
			XFree(childlist);
		}
	}
	if(obj->obj.type==XWINDOW)
		obj->window.focus=NULL;
	else if(obj->obj.parentwindow->window.focus)
		x_object_set_focused(obj->obj.parentwindow);
	XDeleteContext(dis,obj->obj.win,xctxt);
	XDestroyWindow(dis,obj->obj.win);
	switch(obj->obj.type)
	{
	case XWINDOW:
		XFree(obj->window.header.value);
		if(modalwindow==obj)
			modalwindow=NULL;
		break;
	case XBUTTON:
		XFreeGC(dis,obj->button.Black);
		XFreeGC(dis,obj->button.Gray);
		XFreeGC(dis,obj->button.White);
		free(obj->button.text);
		break;
	case XFRAME:
		break;
	case XLABEL:
		free(obj->label.text);
		break;
	}
	free(obj);
}

void x_object_exposed(XObject* obj,XEvent ev)
{

	switch(obj->obj.type)
	{
	case XWINDOW:
		if(obj->window.expose)
			obj->window.expose(obj,ev,obj->obj.cbdata);
		break;
	case XBUTTON:
		x_button_exposed(obj);
		if(obj->button.expose)
			obj->button.expose(obj,ev,obj->obj.cbdata);
		break;
	case XFRAME:
		if(obj->frame.expose)
			obj->frame.expose(obj,ev,obj->obj.cbdata);
		break;
	case XLABEL:
		x_label_exposed(obj);
		break;
	case XTEXTBOX:
		x_textbox_exposed(obj);
		break;
	}
}

void x_object_set_focused(XObject* obj)
{
	XObject * prevfocus=obj->obj.parentwindow->window.focus;
	XEvent ev;
	ev.xfocus.display=dis;
	if(prevfocus)
	{
		prevfocus->obj.isfocused=False;
		ev.type=FocusOut;
		ev.xfocus.window=prevfocus->obj.win;
		XSendEvent(dis,prevfocus->obj.win,False,None,&ev);

	}
	ev.type=FocusIn;
	ev.xfocus.window=obj->obj.win;
	XSendEvent(dis,obj->obj.win,False,None,&ev);
	obj->obj.parentwindow->window.focus=obj;
	obj->obj.isfocused=True;

}

void x_object_move(XObject* obj, int x,int y)
{
	XMoveWindow(dis,obj->obj.win,x,y);
	switch(obj->obj.type)
	{
	case XWINDOW:
		break;
	case XBUTTON:
		obj->button.x=x;
		obj->button.y=y;
		break;
	case XFRAME:
		obj->frame.x=x;
		obj->frame.y=y;
		break;
	case XLABEL:
		obj->label.x=x;
		obj->label.y=y;
		break;
	case XTEXTBOX:
		obj->textbox.x=x;
		obj->textbox.y=y;
	}
}

void x_object_resize(XObject* obj, unsigned width,unsigned height)
{
	if(obj->obj.type!=XLABEL)
	{
		XResizeWindow(dis,obj->obj.win,width,height);
		obj->obj.width=width;
		obj->obj.height=height;
	}
	else
		fprintf(stderr,"Warning: label can not be resized\n");
}
