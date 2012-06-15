#ifndef XOBJECT_H_
#define XOBJECT_H_

#include "X11/Xlib.h"
#include "X11/Xutil.h"
#include "X11/Xos.h"
#include "X11/Xatom.h"
#include <X11/Xresource.h>

typedef struct XToolsGC     XToolsGC    ;
typedef struct XGCInfo      XGCInfo     ;
typedef struct XGCData      XGCData     ;

typedef struct XBasicObject XBasicObject;
typedef struct XWindow      XWindow     ;
typedef struct XButton      XButton     ;
typedef struct XFrame       XFrame      ;
typedef struct XLabel       XLabel      ;
typedef struct XTextBox     XTextBox    ;
typedef struct XScrollBar   XScrollBar  ;

typedef union  XObject      XObject     ;

typedef Bool (*Callback)(XObject *obj,XEvent ev,XPointer cbdata);
typedef void (*CallbackLite)(XObject* obj, XPointer cbdata);

#define XWINDOW  1
#define XBUTTON  2
#define XFRAME   3
#define XLABEL   4
#define XTEXTBOX 5
#define XSCROLL  6


#define OUT_OF_MEMORY 1

struct XGCInfo
{
  unsigned long foreground;
  int line_width;
  int line_style;
};

struct XGCData
{
  GC gc;
  unsigned count;
};

struct XToolsGC
{
  XGCInfo inf;
  XGCData dat;
};

struct XBasicObject
{
	int type;
	Display *dis;
	Window win;
	XPointer cbdata;
	XObject* parentwindow;
	Bool isfocused;
	unsigned long background;
	unsigned width,height;
};

struct XWindow
{
	XBasicObject obj;
	Bool visible;
	XTextProperty header;
	XObject* focus;
	Bool blocked;
	Callback expose,button_press,destroy,configure;
};

struct XButton
{
	XBasicObject obj;
	Bool visible;
	Bool mouse_on,mouse_down;
	int text_width,font_ascent;
	int x,y;
	char* text;
	XToolsGC Black, White,Gray;
	Callback button_press, expose;
};

struct XFrame
{
	XBasicObject obj;
	Bool visible;
	int x,y;
	Callback expose;
};

struct XLabel
{
	XBasicObject obj;
	Bool visible;
	int x,y;
	char* text;
};

struct XTextBox
{
	XBasicObject obj;
	int x, y;
	Bool visible;
	char *buffer;
	int buffer_size;
	int selstart,selend;
	int capacity;
	int font_ascent;
	int text_offset;
	XToolsGC SelectFocused,SelectUnfocused;
	CallbackLite change;
};

struct XScrollBar
{
	XBasicObject obj;
	int x,y;
	Bool visible;
	Bool horizontal;
	int minval,maxval;
	int curval;
	XObject *IncButton, *DecButton;
	unsigned dragwidth,dragheight;
	int dragX,dragY;
	Bool scroll_down;
	int downx, downy;
	XToolsGC Black, White,Gray;
	CallbackLite changed;
};

union XObject
{
	XBasicObject obj;
	XWindow window;
	XButton button;
	XFrame frame;
	XLabel label;
	XTextBox textbox;
	XScrollBar scroll;
};

void x_object_destroy(XObject * obj);
void x_object_exposed(XObject* obj,XEvent ev);
void x_object_set_focused(XObject* obj);
void x_object_move(XObject* obj, int x,int y);
void x_object_resize(XObject* obj, unsigned width,unsigned height);

#endif
