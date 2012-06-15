#include "xscroll.h"
#include "xbutton.h"
#include <stdio.h>
#include <stdlib.h>

extern Display* dis;
extern XContext xctxt;
extern int screen;
extern XrmDatabase xdb;
extern XFontStruct* font;
extern Atom wmDeleteMessage;



XObject* x_scrollbar_create(XObject* parent, int x, int y, unsigned width, 
				unsigned height, Bool visible, Bool horizontal,
				int minval, int maxval,int startval, XPointer cbdata)
{
	XObject* res=(XObject*) calloc(1,sizeof(XObject));
	if(!res)
	{
		fprintf(stderr,"Impossible to create scroll bar: out of memory!\n");
		exit(OUT_OF_MEMORY);
	}
	if(horizontal)
	{
		if(width<3*height)
			width=3*height;
	}
	else
	{
		if(height<3*width)
			height=3*width;
	}
	res->scroll.obj.type=XSCROLL;
	res->scroll.obj.cbdata=cbdata;
	res->scroll.obj.dis=dis;
	res->scroll.obj.background=0xDCDCDC;
	res->scroll.obj.win=XCreateSimpleWindow(dis,parent->obj.win,x,y,width,height,1,0,res->scroll.obj.background);
	if(!res->scroll.obj.win)
	{
		fprintf(stderr,"Failed to create scrollbar\n");
		exit(2);
	}
	res->scroll.obj.parentwindow=parent->obj.parentwindow;
	res->scroll.obj.height=height;
	res->scroll.obj.width=width;
	res->scroll.x=x;
	res->scroll.y=y;
	res->scroll.visible=visible;
	res->scroll.horizontal=horizontal;
	res->scroll.changed=NULL;
	if(minval<=maxval)
	{
		res->scroll.minval=minval;
		res->scroll.maxval=maxval;
	}
	else
	{
		res->scroll.maxval=minval;
		res->scroll.minval=maxval;
	}
	if(horizontal)
	{
		res->scroll.DecButton=x_button_create(res,0,0,height,height,True,"",0,0x7E7E7E,0xFFFFFF,0xDCDCDC,(XPointer)res);
		res->scroll.IncButton=x_button_create(res,width-height,0,height,height,True,"",0,0x7E7E7E,0xFFFFFF,0xDCDCDC,(XPointer)res);
	}
	else
	{
		res->scroll.DecButton=x_button_create(res,0,0,width,width,True,"",0,0x7E7E7E,0xFFFFFF,0xDCDCDC,(XPointer)res);
		res->scroll.IncButton=x_button_create(res,0,height-width,width,width,True,"",0,0x7E7E7E,0xFFFFFF,0xDCDCDC,(XPointer)res);
	}
	x_button_set_expose(res->scroll.DecButton, x_scrollbar_draw_arrow);
	x_button_set_expose(res->scroll.IncButton, x_scrollbar_draw_arrow);
	x_button_set_button_press(res->scroll.DecButton, x_scrollbar_button_press);
	x_button_set_button_press(res->scroll.IncButton, x_scrollbar_button_press);
	x_scrollbar_recalc_size(res);
	res->scroll.White=x_gc_create( res, 0xFFFFFF, 0, LineSolid );
	res->scroll.Black=x_gc_create( res, 0, 0, LineSolid );
	res->scroll.Gray =x_gc_create( res, 0x7E7E7E, 0, LineSolid );
	res->scroll.scroll_down=False;
	XSelectInput(dis,res->scroll.obj.win,KeyPressMask|ExposureMask|ButtonPressMask|ButtonReleaseMask|Button1MotionMask|StructureNotifyMask);
	x_scrollbar_set_value(res,startval);
	if(res->scroll.visible)
		XMapWindow(dis,res->scroll.obj.win);
	XSaveContext(dis,res->obj.win,xctxt,(char*)res);
	return res;
}

void x_scrollbar_recalc_size(XObject* scr)
{
	if(scr->scroll.horizontal)
	{
		scr->scroll.dragheight=scr->scroll.obj.height;
		scr->scroll.dragwidth=(scr->scroll.obj.width-2*scr->scroll.obj.height)/(scr->scroll.maxval-scr->scroll.minval+1);
		if(scr->scroll.dragwidth<5) scr->scroll.dragwidth=5;
		
	}
	else
	{
		scr->scroll.dragwidth=scr->scroll.obj.width;
		scr->scroll.dragheight=(scr->scroll.obj.height-2*scr->scroll.obj.width)/(scr->scroll.maxval-scr->scroll.minval+1);
		if(scr->scroll.dragheight<5) scr->scroll.dragheight=5;
		
	}
}

int x_scrollbar_get_value_by_coord(XObject* scr, int x,int y)
{
	if(scr->scroll.horizontal)
	{
		if(scr->scroll.maxval==scr->scroll.minval)
			return scr->scroll.minval;
		else
			return scr->scroll.minval + (scr->scroll.maxval - scr->scroll.minval) * ((long)( x - (long)scr->scroll.obj.height ) ) / ((long)( scr->scroll.obj.width - scr->scroll.obj.height*2 - scr->scroll.dragwidth ));
	}
	else
	{
		if(scr->scroll.maxval==scr->scroll.minval)
			return scr->scroll.minval;
		else
			return scr->scroll.minval + (scr->scroll.maxval - scr->scroll.minval) * ((long)( y - (long)scr->scroll.obj.width ) ) / ((long)( scr->scroll.obj.height - scr->scroll.obj.width*2 - scr->scroll.dragheight ));
	}
}

int  x_scrollbar_get_value(XObject* scroll)
{
	if(scroll->obj.type==XSCROLL)
		return scroll->scroll.curval;
	else
	{
		fprintf(stderr,"Warning: x_scrollbar_get_value: Wrong type of first argument\n");
		return 0;
	}
}
void  x_scrollbar_set_value(XObject* scroll, int val)
{
	if(scroll->obj.type==XSCROLL)
	{
		if(val > scroll->scroll.maxval)
			val=scroll->scroll.maxval;
		if(val < scroll->scroll.minval)
			val=scroll->scroll.minval;
		if(scroll->scroll.curval != val)
		{
			scroll->scroll.curval=val;
			x_scrollbar_exposed(scroll);
			if(scroll->scroll.changed)
				scroll->scroll.changed(scroll,scroll->obj.cbdata);
		}
	}
	else
	{
		fprintf(stderr,"Warning: x_scrollbar_set_value: Wrong type of first argument\n");
	}
}

int  x_scrollbar_set_range(XObject* scroll, int minval, int maxval,int newval)
{
	if(scroll->obj.type==XSCROLL)
	{
		if(minval<=maxval)
		{
			scroll->scroll.minval=minval;
			scroll->scroll.maxval=maxval;
		}
		else
		{
			scroll->scroll.maxval=minval;
			scroll->scroll.minval=maxval;
		}
		x_scrollbar_recalc_size(scroll);
		scroll->scroll.curval=minval-1;
		x_scrollbar_set_value(scroll,newval);
	}
	else
	{
		fprintf(stderr,"Warning: x_scrollbar_set_range: Wrong type of first argument\n");
		return 0;
	}
}
void x_scrollbar_set_changed(XObject* scroll, CallbackLite f)
{
	if(scroll->obj.type==XSCROLL)
	{
		scroll->scroll.changed=f;
	}
	else
	{
		fprintf(stderr,"Warning: x_scrollbar_set_changed: Wrong type of first argument\n");
	}
}

void x_scrollbar_exposed(XObject* scroll)
{
	XClearWindow(dis,scroll->scroll.obj.win);
	x_scrollbar_draw_slider(scroll);
}

void x_scrollbar_get_slider_coord(XObject* scr, int* dragx, int* dragy)
{

	if(scr->scroll.horizontal)
	{
		if(scr->scroll.maxval==scr->scroll.minval)
		{
			*dragx=scr->scroll.obj.height;
			*dragy=0;
		}
		else
		{
			*dragx=scr->scroll.obj.height+((long)(scr->scroll.obj.width-2*scr->scroll.obj.height-scr->scroll.dragwidth))*(scr->scroll.curval-scr->scroll.minval)/(scr->scroll.maxval-scr->scroll.minval);
			*dragy=0;
		}
	}
	else
	{
		if(scr->scroll.maxval==scr->scroll.minval)
		{
			*dragx=0;
			*dragy=scr->scroll.obj.width;
		}
		else
		{
			*dragx=0;
			*dragy=scr->scroll.obj.width+((long)(scr->scroll.obj.height-2*scr->scroll.obj.width-scr->scroll.dragheight))*(scr->scroll.curval-scr->scroll.minval)/(scr->scroll.maxval-scr->scroll.minval);
		}
	}
}



void x_scrollbar_draw_slider(XObject* scr)
{
	int dragx, dragy;
	x_scrollbar_get_slider_coord(scr,&dragx,&dragy);
	if(scr->scroll.scroll_down)
	{
		x_draw_line(scr,scr->scroll.Black,dragx , dragy, dragx + scr->scroll.dragwidth-1, dragy);
		x_draw_line(scr,scr->scroll.Black,dragx , dragy, dragx , dragy + scr->scroll.dragheight-1);
		x_draw_line(scr,scr->scroll.Gray ,dragx +1 , dragy+1, dragx + scr->scroll.dragwidth-2 , dragy+1);
		x_draw_line(scr,scr->scroll.Gray ,dragx + 1 , dragy+1 , dragx+1 , dragy + scr->scroll.dragheight-2);
		x_draw_line(scr,scr->scroll.White,dragx + scr->scroll.dragwidth-1 , dragy,dragx+scr->scroll.dragwidth-1,dragy+scr->scroll.dragheight-1);
		x_draw_line(scr,scr->scroll.White,dragx,dragy+scr->scroll.dragheight-1,dragx+scr->scroll.dragwidth-1,dragy+scr->scroll.dragheight-1);
	}
	else
	{
		x_draw_line(scr,scr->scroll.White,dragx,dragy,dragx + scr->scroll.dragwidth-1,dragy);
		x_draw_line(scr,scr->scroll.White,dragx,dragy,dragx,dragy+scr->scroll.dragheight-1);
		x_draw_line(scr,scr->scroll.Gray,dragx+scr->scroll.dragwidth-2,dragy+1,dragx+scr->scroll.dragwidth-2,dragy+scr->scroll.dragheight-2);
		x_draw_line(scr,scr->scroll.Gray,dragx+1,dragy+scr->scroll.dragheight-2,dragx+scr->scroll.dragwidth-2,dragy+scr->scroll.dragheight-2);
		x_draw_line(scr,scr->scroll.Black,dragx+scr->scroll.dragwidth-1,dragy,dragx+scr->scroll.dragwidth-1,dragy+scr->scroll.dragheight-1);
		x_draw_line(scr,scr->scroll.Black,dragx,dragy+scr->scroll.dragheight-1,dragx+scr->scroll.dragwidth-1,dragy+scr->scroll.dragheight-1);
	}
}

Bool x_scrollbar_draw_arrow(XObject* btn, XEvent ev, XPointer cbdata)
{
	XObject* parent=(XObject*) cbdata;
	if(parent->obj.type==XSCROLL)
	{
		int type=parent->scroll.horizontal?0:2;
		if(btn==parent->scroll.IncButton)
			type++;
		else if(btn!=parent->scroll.DecButton)
			return False;
		XPoint p[4];
		int h4=btn->obj.height/4;
		int w4=btn->obj.width/4;
		switch(type)
		{
		case 0:
			p[0].x=btn->obj.width/3+1;
			p[0].y=btn->obj.height/2;
			p[1].x=w4;
			p[1].y=-w4;
			p[2].x=0;
			p[2].y=2*w4;
			p[3].x=-p[2].x-p[1].x;
			p[3].y=-p[2].y-p[1].y;
			break;
		case 1:
			p[0].x=2*btn->obj.width/3-1;
			p[0].y=btn->obj.height/2;
			p[1].x=-w4;
			p[1].y=w4;
			p[2].x=0;
			p[2].y=-2*w4;
			p[3].x=-p[2].x-p[1].x;
			p[3].y=-p[2].y-p[1].y;
			break;
		case 2:
			p[0].x=btn->obj.width/2;
			p[0].y=btn->obj.height/3+1;
			p[1].x=-h4;
			p[1].y=h4;
			p[2].x=2*h4;
			p[2].y=0;
			p[3].x=-p[2].x-p[1].x;
			p[3].y=-p[2].y-p[1].y;
			break;
		case 3:
			p[0].x=btn->obj.width/2;
			p[0].y=2*btn->obj.height/3-1;
			p[1].x=h4;
			p[1].y=-h4;
			p[2].x=-2*h4;
			p[2].y=0;
			p[3].x=-p[2].x-p[1].x;
			p[3].y=-p[2].y-p[1].y;
			break;
		}
		if(btn->button.mouse_on&&btn->button.mouse_down)
		{
			p[0].x++;
			p[0].y++;
		}	
		XDrawLines(btn->obj.dis,btn->obj.win,btn->button.Black.dat.gc,p,4,CoordModePrevious);
		XFillPolygon(btn->obj.dis,btn->obj.win,btn->button.Black.dat.gc,p,4,Convex,CoordModePrevious);
		
	}
	return True;
}

Bool x_scrollbar_button_press(XObject* btn, XEvent ev, XPointer cbdata)
{
	XObject* parent=(XObject*) cbdata;
	if(parent->obj.type==XSCROLL)
	{
		int type=parent->scroll.horizontal?0:2;
		if(btn==parent->scroll.IncButton)
			type++;
		else if(btn!=parent->scroll.DecButton)
			return False;
		if((type&1)==0)
		{
			x_scrollbar_set_value(parent,parent->scroll.curval-1);
		}
		else
		{
			x_scrollbar_set_value(parent,parent->scroll.curval+1);
		}
		
	}
	return True;
}

void x_scrollbar_handle_event(XObject* obj, XEvent event)
{
	XEvent ev;
	int dragx, dragy;
	switch(event.type)
	{
	case Expose:
		x_object_exposed(obj,event);
		break;
	case ButtonPress:
		if(event.xbutton.button==Button1)
		{
			x_scrollbar_get_slider_coord(obj,&dragx,&dragy);
			if( ( dragx <= event.xbutton.x && event.xbutton.x <= dragx + obj->scroll.dragwidth ) &&
			    ( dragy <= event.xbutton.y && event.xbutton.y <= dragy + obj->scroll.dragheight ) )
			{
				obj->scroll.scroll_down=True;
				obj->scroll.downx=event.xbutton.x;
				obj->scroll.downy=event.xbutton.y;
				x_object_exposed(obj,event);
			}
		}
		break;
	case ButtonRelease:
		if(event.xbutton.button==Button1)
		{
			if(obj->scroll.scroll_down==True)
			{
				obj->scroll.scroll_down=False;
				x_object_exposed(obj,event);
			}
		}
		break;
	case MotionNotify:
		if(obj->scroll.scroll_down)
			x_scrollbar_set_value(obj, x_scrollbar_get_value_by_coord( obj, event.xbutton.x, event.xbutton.y ) );
		break;
	case ConfigureNotify:
		if(obj->scroll.horizontal)
		{
			if(obj->obj.width<3*obj->obj.height)
			{
				x_object_resize(obj,3*obj->obj.height,obj->obj.height);
				break;
			}
		}
		else
		{
			if(obj->obj.height<3*obj->obj.width)
			{
				x_object_resize(obj,obj->obj.width,3*obj->obj.width);
				break;
			}
		}
		if(obj->scroll.horizontal)
		{
			x_object_resize(obj->scroll.DecButton,obj->obj.height,obj->obj.height);
			x_object_resize(obj->scroll.IncButton,obj->obj.height,obj->obj.height);
			x_object_move(obj->scroll.IncButton,obj->obj.width - obj->obj.height,0);
		}
		else
		{
			x_object_resize(obj->scroll.DecButton,obj->obj.width,obj->obj.width);
			x_object_resize(obj->scroll.IncButton,obj->obj.width,obj->obj.width);
			x_object_move(obj->scroll.IncButton,0,obj->obj.height - obj->obj.width);
		}
		x_scrollbar_recalc_size(obj);
		x_scrollbar_exposed(obj);
		break;
		
	}
}
