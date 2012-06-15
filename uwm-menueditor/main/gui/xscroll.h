#ifndef XSCROLL_H_
#define XSCROLL_H_

#include "xobject.h"
#include "xtools.h"

XObject* x_scrollbar_create(XObject* parent, int x, int y, unsigned width, 
				unsigned height, Bool visible, Bool horizontal,
				int minval, int maxval,int startval, XPointer cbdata);
void x_scrollbar_recalc_size(XObject* scr);
int  x_scrollbar_get_value(XObject* scroll);
int x_scrollbar_get_value_by_coord(XObject* scr, int x,int y);
void  x_scrollbar_set_value(XObject* scroll, int val);
int  x_scrollbar_set_range(XObject* scroll, int minval, int maxval,int newval);
void x_scrollbar_set_changed(XObject* scroll, CallbackLite f);
void x_scrollbar_exposed(XObject* scroll);
void x_scrollbar_get_slider_coord(XObject* scr, int* dragx, int* dragy);
void x_scrollbar_draw_slider(XObject* scr);
Bool x_scrollbar_draw_arrow(XObject* btn, XEvent ev, XPointer cbdata);
Bool x_scrollbar_button_press(XObject* btn, XEvent ev, XPointer cbdata);
void x_scrollbar_handle_event(XObject* scr, XEvent event);

#endif
