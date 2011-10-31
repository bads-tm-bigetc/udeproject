#include "xlabel.h"
#include <stdio.h>
#include <stdlib.h>

extern Display* dis;
extern XContext xctxt;
extern int screen;
extern XrmDatabase xdb;
extern XFontStruct* font;
extern Atom wmDeleteMessage;

XObject* x_label_create (XObject* parent, int x, int y, char * text, Bool visible,XPointer cbdata)
{
	int len=strlen(text);
	XObject * l=(XObject*)calloc(1,sizeof(XObject));
	if(!l)
	{
		fprintf(stderr,"Impossible to create label: out of memory!\n");
		exit(OUT_OF_MEMORY);
	}
	l->obj.type=XLABEL;
	l->obj.cbdata=cbdata;
	l->obj.dis=dis;
	l->label.text=(char*)calloc(len+1,sizeof(char));
	if(!l->label.text)
	{
		fprintf(stderr,"Impossible to create label: out of memory!\n");
		exit(OUT_OF_MEMORY);
	}
	strcpy(l->label.text,text);
	l->obj.width=XTextWidth(font,l->label.text,len);
	if(l->obj.width<5)
		l->obj.width=5;
	l->obj.height=font->ascent+font->descent+2;
	l->obj.background=parent->obj.background;
	l->label.x=x;
	l->label.y=y;
	l->obj.win=XCreateSimpleWindow(dis,parent->obj.win,x,y,l->obj.width,l->obj.height,0,0,l->obj.background);
	if(!l->obj.win)
	{
		fprintf(stderr,"Failed to create window\n");
		exit(2);
	}
	l->obj.parentwindow=parent->obj.parentwindow;
	l->obj.isfocused=False;
	l->label.visible=visible;
	XSelectInput(dis,l->obj.win,ExposureMask|KeyPressMask);
	if(l->label.visible)
		XMapWindow(dis,l->obj.win);

	XSaveContext(dis,l->obj.win,xctxt,(char*)l);
	return l;
}

void x_label_exposed(XObject * l)
{
	XClearWindow(dis,l->obj.win);
	XDrawString(dis,l->obj.win,DefaultGC(dis,screen),1,font->ascent,l->label.text,strlen(l->label.text));
}

void x_label_set_text(XObject * l,char * text)
{
	if(l->obj.type==XLABEL)
	{
		int len=strlen(text);
		free(l->label.text);
		l->label.text=(char*)calloc(len+1,sizeof(char));
		if(!l->label.text)
		{
			fprintf(stderr,"Impossible to change label text: Out of memory\n");
			exit(OUT_OF_MEMORY);
		}
		strcpy(l->label.text,text);
		l->obj.width=XTextWidth(font,l->label.text,len);
		if(l->obj.width<5)
			l->obj.width=5;
		l->obj.height=font->ascent+font->descent+2;
		XResizeWindow(dis,l->label.obj.win,l->obj.width,l->obj.height);
		x_label_exposed(l);
	}
	else
		fprintf(stderr,"Warning: x_label_set_text: Wrong type of first argument");
}

void x_label_set_visible(XObject * l, Bool visible)
{
	if(l->obj.type==XLABEL)
	{
		if(l->label.visible!=visible)
		{
			l->label.visible=visible;
			if(l->label.visible)
				XMapWindow(dis,l->label.obj.win);
			else
				XUnmapWindow(dis,l->label.obj.win);
		}
	}
	else
		fprintf(stderr,"Warning: x_label_set_visible: Wrong type of first argument\n");
}

void x_label_handle_event(XObject* obj, XEvent event)
{
	if(obj->label.visible)
		switch(event.type)
		{
		case Expose:
			x_object_exposed(obj,event);
			break;
		}
}
