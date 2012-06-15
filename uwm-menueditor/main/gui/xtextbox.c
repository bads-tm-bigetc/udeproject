#include <stdio.h>
#include <stdlib.h>
#include "xtextbox.h"
#include "xobject.h"

extern Display* dis;
extern XContext xctxt;
extern int screen;
extern XrmDatabase xdb;
extern XFontStruct* font;
extern Atom wmDeleteMessage;

extern Bool paste_requested;
extern Window selowner;
extern XObject* pasteexp;
extern int pastepos;

extern Atom sel_type;
extern char* sel_text;
static char buff[256];

XObject* x_textbox_create(XObject* parent, int x, int y, unsigned width, unsigned height, Bool visible,
		XPointer cbdata)
{
	XObject* t=(XObject*) calloc(1,sizeof(XObject));
	if(!t)
	{
		fprintf(stderr,"Impossible to create textbox: out of memory!\n");
		exit(OUT_OF_MEMORY);
	}
	t->textbox.obj.type=XTEXTBOX;
	t->textbox.obj.cbdata=cbdata;
	t->textbox.obj.dis=dis;
	t->textbox.obj.background=0xFFFFFF;
	t->textbox.obj.win=XCreateSimpleWindow(dis,parent->obj.win,x,y,width,height,1,0,0xFFFFFF);
	if(!t->textbox.obj.win)
	{
		fprintf(stderr,"Failed to create textbox\n");
		exit(2);
	}
	t->obj.isfocused=False;
	t->textbox.obj.parentwindow=parent->obj.parentwindow;
	t->textbox.change=NULL;
	t->obj.height=height;
	t->obj.width=width;
	t->textbox.x=x;
	t->textbox.y=y;
	t->textbox.capacity=256;
	t->textbox.selstart=t->textbox.selend=0;
	t->textbox.buffer=(char*)calloc(t->textbox.capacity,sizeof(char));
	if(!t->textbox.buffer)
	{
		fprintf(stderr,"Impossible to create textbox: out of memory!\n");
		exit(OUT_OF_MEMORY);
	}
	t->textbox.buffer_size=0;
	t->textbox.font_ascent=font->ascent;
	t->textbox.text_offset=0;
	t->textbox.SelectFocused=x_gc_create(t,0x4e84b2,0,LineSolid);
	t->textbox.SelectUnfocused=x_gc_create(t,0xA7A7A7,0,LineSolid);
	XSelectInput(dis,t->textbox.obj.win,ExposureMask|ButtonPressMask|
			ButtonReleaseMask|KeyPressMask|Button1MotionMask|StructureNotifyMask);
	t->textbox.visible=visible;
	if(t->textbox.visible)
		XMapWindow(dis,t->textbox.obj.win);
	XSaveContext(dis,t->obj.win,xctxt,(char*)t);
	return t;
}

void x_textbox_exposed(XObject* t)
{
	int i,w;
	int s1=XTextWidth(font,t->textbox.buffer,t->textbox.selstart)+2;
	int s2=XTextWidth(font,t->textbox.buffer,t->textbox.selend)+2;
	w=s2;
	if(s1>s2)
	{
		i=s1;
		s1=s2;
		s2=i;
	}
	XClearWindow(dis,t->obj.win);
	x_fill_rectangle(t,t->obj.isfocused?t->textbox.SelectFocused:t->textbox.SelectUnfocused,s1-t->textbox.text_offset,1,s2-s1,font->ascent+font->descent);
	XDrawString(dis,t->obj.win,DefaultGC(dis,screen),2-t->textbox.text_offset,font->ascent+1,t->textbox.buffer,strlen(t->textbox.buffer));
	if(t->obj.isfocused)
		XDrawLine(dis,t->obj.win,DefaultGC(dis,screen),w-t->textbox.text_offset,1,w-t->textbox.text_offset,font->ascent+font->descent-1);
}

void x_textbox_erase_text(XObject* t, int l, int r)
{
	int i;
	if(l>r)
	{
		i=l;
		l=r;
		r=i;
	}
	r--;
	if(l<=t->textbox.selstart&&t->textbox.selstart<=r)
		t->textbox.selstart=l;
	else if(r<t->textbox.selstart)
		t->textbox.selstart-=(r-l+1);
	if(l<=t->textbox.selend&&t->textbox.selend<=r)
		t->textbox.selend=l;
	else if(r<t->textbox.selend)
		t->textbox.selend-=(r-l+1);
	for(i=l;i+(r-l+1)<t->textbox.buffer_size;i++)
		t->textbox.buffer[i]=t->textbox.buffer[i+(r-l+1)];
	t->textbox.buffer_size-=(r-l+1);
	t->textbox.buffer[t->textbox.buffer_size]='\0';
}

void x_textbox_insert_text(XObject* t,int pos,char* str, int len)
{
	int i;
	Bool toosmall=False;
	while(t->textbox.buffer_size+len>t->textbox.capacity)
	{
		t->textbox.capacity*=2;
		toosmall=True;
	}
	if(toosmall)
		if((t->textbox.buffer=(char*)realloc(t->textbox.buffer,t->textbox.capacity))==NULL)
		{
			fprintf(stderr,"Unable to expand textbox buffer: Out of memory!\n");
			exit(OUT_OF_MEMORY);
		}
	{
		if(t->textbox.selstart>=pos)
			t->textbox.selstart+=len;
		if(t->textbox.selend>=pos)
			t->textbox.selend+=len;
		for(i=t->textbox.buffer_size-1;i>=pos;i--)
			t->textbox.buffer[i+len]=t->textbox.buffer[i];
		for(i=0;i<len;i++)
			t->textbox.buffer[i+pos]=str[i];
		t->textbox.buffer_size+=len;
		t->textbox.buffer[t->textbox.buffer_size]='\0';
	}
}

void x_textbox_move_cursor_left(XObject* t,Bool Shift)
{
	if(t->textbox.selend>0)
		t->textbox.selend--;
	if(!Shift)
		t->textbox.selstart=t->textbox.selend;
}

void x_textbox_move_cursor_right(XObject* t,Bool Shift)
{
	if(t->textbox.selend<t->textbox.buffer_size)
		t->textbox.selend++;
	if(!Shift)
			t->textbox.selstart=t->textbox.selend;
}

void x_textbox_copy_selected(XObject* t)
{
	int l,r,i;
	l=t->textbox.selstart;
	r=t->textbox.selend;
	if(l!=r)
	{
		if(l>r)
		{
			i=l;
			l=r;
			r=i;
		}
		if(sel_text!=NULL)
			free(sel_text);
		sel_type=XA_STRING;
		sel_text=(char*) calloc(r-l+1,sizeof(char));
		memcpy(sel_text,&t->textbox.buffer[l],r-l);
		XSetSelectionOwner (dis, XA_PRIMARY, t->obj.parentwindow->obj.win, CurrentTime);
	}
}

void x_textbox_paste(XObject* t,int pos)
{
	Atom type;
	unsigned long int len,bytes_left,dummy;
	int result,format;
	unsigned char* clipboard;
	XGetWindowProperty (dis, t->obj.win,XA_STRING, 0, 0, 0, AnyPropertyType, &type, &format, &len, &bytes_left, &clipboard);
	if(bytes_left>0)
	{
		result=XGetWindowProperty(dis,t->obj.win,XA_STRING,0,bytes_left,0,AnyPropertyType,&type,&format,&len,&dummy,&clipboard);
		if(result==Success)
		{
			x_textbox_insert_text(t,pos,(char*)clipboard,bytes_left);
			XFree(clipboard);
		}
	}
}

void x_textbox_set_change(XObject* t, CallbackLite f)
{
	if(t->obj.type==XTEXTBOX)
	{
		t->textbox.change=f;
	}
	else
		fprintf(stderr,"Warning: x_textbox_set_change: Wrong type of first argument\n");
}

void x_textbox_set_text(XObject *t, char* text)
{
	if(t->obj.type==XTEXTBOX)
	{
		strcpy(t->textbox.buffer,text);
		t->textbox.selstart=t->textbox.selend=t->textbox.buffer_size=strlen(text);
		x_textbox_exposed(t);
		if(t->textbox.change)
			t->textbox.change(t,t->obj.cbdata);
	}
	else
		fprintf(stderr,"Warning: x_textbox_set_text: Wrong type of first argument\n");
}

void x_textbox_update_offset(XObject* t)
{
	int w=XTextWidth(font,t->textbox.buffer,t->textbox.buffer_size);
	//if everything can be visible than we need no offset
	if(w < t->obj.width-2)
		t->textbox.text_offset=0;
	else
	{
		int len=XTextWidth(font,t->textbox.buffer,t->textbox.selend);
		//if cursor is out of screen
		if((len>=t->textbox.text_offset)&&(len-t->textbox.text_offset>t->obj.width-2))
		{
			//beyond right bound or the right end of text is seen
			t->textbox.text_offset=2+len-t->obj.width + 1;
		}
		else if(len-t->textbox.text_offset-2<0)
		{
			//beyond left bound
			t->textbox.text_offset=len + 2;
		}
		else if((w>=t->textbox.text_offset)&&(w-t->textbox.text_offset<t->obj.width))
		{
			t->textbox.text_offset=2+w-t->obj.width + 1;
		}
	}

}

void x_textbox_set_visible(XObject * t, Bool visible)
{
	if(t->obj.type==XTEXTBOX)
	{
		if(t->textbox.visible!=visible)
		{
			t->textbox.visible=visible;
			if(t->textbox.visible)
				XMapWindow(dis,t->textbox.obj.win);
			else
				XUnmapWindow(dis,t->textbox.obj.win);
		}
	}
	else
		fprintf(stderr,"Warning: x_textbox_set_visible: Wrong type of first argument\n");
}

void x_textbox_handle_event(XObject* obj,XEvent event)
{
	KeySym ks;
	int len,x;
	Bool is_shift_pressed,changed;
	int pos;
	int i;
	switch(event.type)
	{
	case SelectionNotify:
		x_textbox_paste(obj,pastepos);
		x_textbox_exposed(obj);
		if(obj->textbox.change)
			obj->textbox.change(obj,obj->obj.cbdata);
		break;
	case KeyPress:
		for(i=0;i<255;i++)
			buff[i]=0;
		XLookupString(&(event.xkey),buff,255,&ks,NULL);
		len=strlen(buff);
		if(ks==XK_BackSpace)
		{
			if(obj->textbox.selstart==obj->textbox.selend)
			{
				if(obj->textbox.selstart>0)
				{
					x_textbox_erase_text(obj,obj->textbox.selstart-1,obj->textbox.selstart);
					x_textbox_update_offset(obj);
					x_textbox_exposed(obj);
					if(obj->textbox.change)
						obj->textbox.change(obj,obj->obj.cbdata);
				}
			}
			else
			{
				x_textbox_erase_text(obj,obj->textbox.selstart,obj->textbox.selend);
				x_textbox_update_offset(obj);
				x_textbox_exposed(obj);
				if(obj->textbox.change)
					obj->textbox.change(obj,obj->obj.cbdata);
			}
		}
		else if(ks==XK_Delete)
		{
			if(obj->textbox.selstart==obj->textbox.selend)
			{
				if(obj->textbox.selstart<obj->textbox.buffer_size)
				{
					x_textbox_erase_text(obj,obj->textbox.selstart,obj->textbox.selstart+1);
					x_textbox_update_offset(obj);
					x_textbox_exposed(obj);
					if(obj->textbox.change)
						obj->textbox.change(obj,obj->obj.cbdata);
				}
			}
			else
			{
				x_textbox_erase_text(obj,obj->textbox.selstart,obj->textbox.selend);
				x_textbox_update_offset(obj);
				x_textbox_exposed(obj);
				if(obj->textbox.change)
					obj->textbox.change(obj,obj->obj.cbdata);
			}
		}
		else if(ks==XK_Left)
		{
			x_textbox_move_cursor_left(obj,(event.xkey.state&ShiftMask)?True:False);
			x_textbox_copy_selected(obj);
			x_textbox_update_offset(obj);
			x_textbox_exposed(obj);
		}
		else if(ks==XK_Right)
		{
			x_textbox_move_cursor_right(obj,(event.xkey.state&ShiftMask)?True:False);
			x_textbox_copy_selected(obj);
			x_textbox_update_offset(obj);
			x_textbox_exposed(obj);
		}
		else if(ks==XK_End)
		{
			changed=False;
			is_shift_pressed=(event.xkey.state&ShiftMask)?True:False;
			while(obj->textbox.selend<obj->textbox.buffer_size)
			{
				x_textbox_move_cursor_right(obj,is_shift_pressed);
				changed=True;
			}
			if(!is_shift_pressed&&obj->textbox.selstart!=obj->textbox.selend)
			{
				obj->textbox.selstart=obj->textbox.selend;
				changed=True;
			}
			x_textbox_copy_selected(obj);
			if(changed)
			{
				x_textbox_update_offset(obj);
				x_textbox_exposed(obj);
			}
		}
		else if(ks==XK_Home)
		{
			changed=False;
			is_shift_pressed=(event.xkey.state&ShiftMask)?True:False;
			while(obj->textbox.selend>0)
			{
				x_textbox_move_cursor_left(obj,is_shift_pressed);
				changed=True;
			}
			if(!is_shift_pressed&&obj->textbox.selstart!=obj->textbox.selend)
			{
				obj->textbox.selstart=obj->textbox.selend;
				changed=True;
			}
			x_textbox_copy_selected(obj);
			if(changed)
			{
				x_textbox_update_offset(obj);
				x_textbox_exposed(obj);
			}
		}
		else if(len>0)
		{
			if(obj->textbox.selstart!=obj->textbox.selend)
				x_textbox_erase_text(obj,obj->textbox.selstart,obj->textbox.selend);
			x_textbox_insert_text(obj,obj->textbox.selend,buff,len);
			x_textbox_update_offset(obj);
			x_textbox_exposed(obj);
			if(obj->textbox.change)
				obj->textbox.change(obj,obj->obj.cbdata);
		}
		break;
	case Expose:
		x_object_exposed(obj,event);
		break;
	case ButtonPress:
		if(event.xbutton.button==Button1)
		{
			is_shift_pressed=(event.xbutton.state&ShiftMask)?True:False;
			changed=False;
			x=event.xbutton.x;
			if(!is_shift_pressed)
			{
				if(obj->textbox.selstart!=obj->textbox.selend)
				{
					changed=True;
					obj->textbox.selstart=obj->textbox.selend;
				}
			}
			//Seek new cursor position
			while(XTextWidth(font,obj->textbox.buffer,obj->textbox.selend)+2-obj->textbox.text_offset>x &&
					obj->textbox.selend>0)
			{
				x_textbox_move_cursor_left(obj,is_shift_pressed);
				changed=True;
			}
			while(XTextWidth(font,obj->textbox.buffer,obj->textbox.selend+1)+2-obj->textbox.text_offset<x &&
					obj->textbox.selend<obj->textbox.buffer_size)
			{
				x_textbox_move_cursor_right(obj,is_shift_pressed);
				changed=True;
			}
			//Redraw if cursor position changed
			if(changed)
			{
				x_textbox_copy_selected(obj);
				x_textbox_update_offset(obj);
				x_textbox_exposed(obj);
			}
		}
		else if(event.xbutton.button==Button2)
		{
			x=event.xbutton.x;
			for(pos=0;pos<obj->textbox.buffer_size;pos++)
			{
				if(XTextWidth(font,obj->textbox.buffer,pos)+2-obj->textbox.text_offset>x)
				{
					if(pos>0)
						pos--;
					break;
				}
			}

			selowner=XGetSelectionOwner(dis,XA_PRIMARY);
			if(selowner!=None)
			{
				paste_requested=True;
				pasteexp=obj;
				pastepos=pos;
				XConvertSelection(dis,XA_PRIMARY,XA_STRING,XA_STRING ,obj->obj.win,CurrentTime);
			}

		}
		break;
	case MotionNotify:
		changed=False;
		x=event.xbutton.x;
		while(XTextWidth(font,obj->textbox.buffer,obj->textbox.selend)+1-obj->textbox.text_offset>x &&
				obj->textbox.selend>0)
		{
			x_textbox_move_cursor_left(obj,True);
			changed=True;
		}
		while(XTextWidth(font,obj->textbox.buffer,obj->textbox.selend+1)+1-obj->textbox.text_offset<x &&
				obj->textbox.selend<obj->textbox.buffer_size)
		{
			x_textbox_move_cursor_right(obj,True);
			changed=True;
		}
		if(changed)
		{
			x_textbox_copy_selected(obj);
			x_textbox_update_offset(obj);
			x_textbox_exposed(obj);
		}
		break;
	case FocusIn:
	case FocusOut:
		x_textbox_exposed(obj);
		break;
	case ConfigureNotify:
		x_textbox_update_offset(obj);
		x_textbox_exposed(obj);
		break;
	}
}
