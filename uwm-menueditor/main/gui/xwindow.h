#ifndef XWINDOW_H_
#define XWINDOW_H_

#include "xobject.h"

XObject* x_window_create(unsigned width,unsigned height,
		Bool visible,unsigned bordersize, unsigned long int bordercolor,
		unsigned long int background, char* title, XPointer cbdata);
XObject* x_window_create_transient(XObject* parent, unsigned width,unsigned height,
		Bool visible,unsigned bordersize, unsigned long int bordercolor,
		unsigned long int background, char* title, XPointer cbdata);
XObject* x_window_get_transient(XObject* w);
void x_window_set_visible(XObject* w, Bool visible);
void x_window_set_modal(XObject* w,Bool modal);
void x_window_set_blocked(XObject* w, Bool blocked);
void x_window_set_expose(XObject* w, Callback f);
void x_window_set_button_press(XObject* w, Callback f);
void x_window_set_destroy_event(XObject* w, Callback f);
void x_window_set_configure(XObject* w, Callback f);
void x_window_handle_event(XObject* obj, XEvent event);
#endif
