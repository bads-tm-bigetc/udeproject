#ifndef XLABEL_H_
#define XLABEL_H_
#include "xobject.h"

XObject* x_label_create (XObject* parent, int x, int y, char * text, Bool Visible, XPointer cbdata);
void x_label_exposed(XObject * l);
void x_label_set_text(XObject * l,char * text);
void x_label_set_visible(XObject * l, Bool visible);
void x_label_handle_event(XObject* obj, XEvent event);
#endif
