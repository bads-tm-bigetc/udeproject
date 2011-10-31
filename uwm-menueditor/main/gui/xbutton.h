#ifndef XBUTTON_H_
#define XBUTTON_H_

#include "xobject.h"
XObject* x_button_create(XObject *parent,int x,int y, unsigned width,
		                unsigned height, Bool visible,char* caption,
		                unsigned long black, unsigned long gray, unsigned long white,
		                unsigned long background,XPointer cbdata);
void x_button_set_button_press(XObject* b,Callback f);
void x_button_set_expose(XObject* b, Callback f);
void x_button_exposed(XObject* b);
void x_button_redraw_border(XObject* b);
void x_button_set_visible(XObject* b, Bool visible);
void x_button_set_text(XObject* b, char* text);
void x_button_handle_event(XObject* b, XEvent event);
#endif
