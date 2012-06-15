#include "xbutton.h"
#include "xdraw.h"
#include "xtoolsgc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Display* dis;
extern XContext xctxt;
extern int screen;
extern XrmDatabase xdb;
extern XFontStruct* font;
extern Atom wmDeleteMessage;

XObject* x_button_create(XObject *parent,int x,int y, unsigned width,
		                unsigned height, Bool visible,char* caption,
		                unsigned long black, unsigned long gray, unsigned long white,
		                unsigned long background,XPointer cbdata)
{
	XObject *b=(XObject*) calloc(1,sizeof(XObject));
	if(!b)
	{
		fprintf(stderr,"Impossible to create button: out of memory!\n");
		exit(OUT_OF_MEMORY);
	}
	b->button.obj.type=XBUTTON;
	b->button.obj.cbdata=cbdata;
	b->button.obj.dis=dis;
	b->button.obj.background=background;
	b->button.obj.win=XCreateSimpleWindow(dis,parent->obj.win,x,y,width,height,0,0,b->button.obj.background);
	if(!b->button.obj.win)
	{
		fprintf(stderr,"Failed to create button\n");
		exit(2);
	}
	b->button.obj.parentwindow=parent->obj.parentwindow;
	b->button.button_press=NULL;
	b->obj.height=height;
	b->obj.width=width;
	b->button.x=x;
	b->button.y=y;
	b->button.text=(char*)calloc(strlen(caption)+1,sizeof(char));
	if(!b->button.text)
	{
		fprintf(stderr,"Impossible to create button: out of memory!\n");
		exit(OUT_OF_MEMORY);
	}
	strcpy(b->button.text,caption);
	b->button.White=x_gc_create( b, white, 0, LineSolid );
	b->button.Black=x_gc_create( b, black, 0, LineSolid );
	b->button.Gray =x_gc_create( b, gray, 0, LineSolid );
	b->button.text_width=XTextWidth(font,b->button.text,strlen(b->button.text));
	b->button.font_ascent=font->ascent;
	XSelectInput(dis,b->button.obj.win,KeyPressMask|ExposureMask|ButtonPressMask|
			ButtonReleaseMask|EnterWindowMask|LeaveWindowMask|VisibilityChangeMask);
	b->obj.isfocused=False;
	b->button.visible=visible;
	b->button.mouse_on=False;
	b->button.mouse_down=False;
	if(b->button.visible)
		XMapWindow(dis,b->button.obj.win);
	XSaveContext(dis,b->obj.win,xctxt,(char*)b);
	return b;
}

void x_button_exposed(XObject* b)
{
	int textx,texty;
	XClearWindow(dis,b->button.obj.win);
	x_button_redraw_border(b);
	textx = (b->obj.width - b->button.text_width)/2;
	texty = (b->obj.height + b->button.font_ascent)/2;
	XDrawString(dis,b->button.obj.win,DefaultGC(dis,screen),textx,texty,b->button.text,strlen(b->button.text));
}

void x_button_redraw_border(XObject* b)
{
	if(b->button.mouse_on&&b->button.mouse_down)
	{
		x_draw_line(b,b->button.Black,0,0,b->obj.width-1,0);	  
		x_draw_line(b,b->button.Black,0,0,0,b->obj.height-1);
		x_draw_line(b,b->button.Gray,1,1,b->obj.width-2,1);
		x_draw_line(b,b->button.Gray,1,1,1,b->obj.height-2);
		x_draw_line(b,b->button.White,b->obj.width-1,0,b->obj.width-1,b->obj.height-1);
		x_draw_line(b,b->button.White,0,b->obj.height-1,b->obj.width-1,b->obj.height-1);
	}
	else
	{
		x_draw_line(b,b->button.White,0,0,b->obj.width-1,0);
		x_draw_line(b,b->button.White,0,0,0,b->obj.height-1);
		x_draw_line(b,b->button.Gray,b->obj.width-2,1,b->obj.width-2,b->obj.height-2);
		x_draw_line(b,b->button.Gray,1,b->obj.height-2,b->obj.width-2,b->obj.height-2);
		x_draw_line(b,b->button.Black,b->obj.width-1,0,b->obj.width-1,b->obj.height-1);
		x_draw_line(b,b->button.Black,0,b->obj.height-1,b->obj.width-1,b->obj.height-1);
	}
}

void x_button_set_button_press(XObject* b,Callback f)
{
	if(b->obj.type==XBUTTON)
	{
		b->button.button_press=f;
	}
	else
		fprintf(stderr,"Warning: x_button_set_button_press: Wrong type of first argument\n");
}

void x_button_set_visible(XObject* b, Bool visible)
{
	if(b->obj.type==XBUTTON)
	{
		if(b->button.visible!=visible)
		{
			b->button.visible=visible;
			if(b->button.visible)
				XMapWindow(dis,b->button.obj.win);
			else
			{
				XUnmapWindow(dis,b->button.obj.win);
				b->button.mouse_down=False;
				b->button.mouse_on=False;
			}
		}
	}
	else
		fprintf(stderr,"Warning: x_button_set_visible: Wrong type of first argument\n");
}

void x_button_set_expose(XObject* b, Callback f)
{
	if(b->obj.type==XBUTTON)
	{
		b->button.expose=f;
	}
	else
		fprintf(stderr,"Warning: x_button_set_expose: Wrong type of first argument\n");
}

void x_button_set_text(XObject* b, char* text)
{
	if(b->obj.type==XBUTTON)
	{
		free(b->button.text);
		b->button.text=(char*)calloc(strlen(text)+1,sizeof(char));
		if(!b->button.text)
		{
			fprintf(stderr,"Impossible to change button text: out of memory!\n");
			exit(OUT_OF_MEMORY);
		}
		strcpy(b->button.text,text);
		b->button.text_width=XTextWidth(font,b->button.text,strlen(b->button.text));
		XEvent ev;
		ev.xexpose.type=Expose;
		ev.xexpose.display=dis;
		ev.xexpose.count=1;
		ev.xexpose.x=0;
		ev.xexpose.y=0;
		ev.xexpose.width=b->obj.width;
		ev.xexpose.height=b->obj.height;
		ev.xexpose.window=b->obj.win;
		x_object_exposed(b,ev);
	}
	else
		fprintf(stderr,"Warning: x_button_set_text: Wrong type of first argument\n");
}

void x_button_handle_event(XObject* obj, XEvent event)
{
	XEvent ev;
	if(obj->button.visible)
		switch(event.type)
		{
		case Expose:
			x_object_exposed(obj,event);
			break;
		case ButtonPress:
			if(event.xbutton.button==Button1)
			{
				obj->button.mouse_down=True;
				obj->button.mouse_on=True;
				ev.xexpose.type=Expose;
				ev.xexpose.display=dis;
				ev.xexpose.count=1;
				ev.xexpose.x=0;
				ev.xexpose.y=0;
				ev.xexpose.width=obj->obj.width;
				ev.xexpose.height=obj->obj.height;
				ev.xexpose.window=obj->obj.win;
				x_object_exposed(obj,ev);
			}
			break;
		case ButtonRelease:
			if(event.xbutton.button==Button1)
			{
				obj->button.mouse_down=False;
				if(obj->button.mouse_on)
				{
					ev.xexpose.type=Expose;
					ev.xexpose.display=dis;
					ev.xexpose.count=1;
					ev.xexpose.x=0;
					ev.xexpose.y=0;
					ev.xexpose.width=obj->obj.width;
					ev.xexpose.height=obj->obj.height;
					ev.xexpose.window=obj->obj.win;
					x_object_exposed(obj,ev);
				}
				if(obj->button.mouse_on&&obj->button.button_press)
					obj->button.button_press(obj,event,obj->obj.cbdata);
			}
			break;
		case EnterNotify:
			obj->button.mouse_on=True;
			if(obj->button.mouse_down)
			{
				ev.xexpose.type=Expose;
				ev.xexpose.display=dis;
				ev.xexpose.count=1;
				ev.xexpose.x=0;
				ev.xexpose.y=0;
				ev.xexpose.width=obj->obj.width;
				ev.xexpose.height=obj->obj.height;
				ev.xexpose.window=obj->obj.win;
				x_object_exposed(obj,ev);
			}
			break;
		case LeaveNotify:
			obj->button.mouse_on=False;
			if(obj->button.mouse_down)
			{
				XEvent ev;
				ev.xexpose.type=Expose;
				ev.xexpose.display=dis;
				ev.xexpose.count=1;
				ev.xexpose.x=0;
				ev.xexpose.y=0;
				ev.xexpose.width=obj->obj.width;
				ev.xexpose.height=obj->obj.height;
				ev.xexpose.window=obj->obj.win;
				x_object_exposed(obj,ev);
			}
			break;
		case VisibilityNotify:
			obj->button.mouse_on=False;
			obj->button.mouse_down=False;
			break;
		}
}
