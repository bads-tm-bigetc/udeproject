#ifndef XFRAME_H_
#define XFRAME_H_
#include "xobject.h"

XObject* x_frame_create(XObject* parent, int x, int y, unsigned width, unsigned height,
		Bool visible, unsigned bordersize, unsigned long bordercolor, unsigned long background,
		XPointer cbdata);

void x_frame_set_visible(XObject* f, Bool visible);
void x_frame_set_expose(XObject* fr, Callback f);
void x_frame_handle_event(XObject* obj, XEvent event);

#endif
