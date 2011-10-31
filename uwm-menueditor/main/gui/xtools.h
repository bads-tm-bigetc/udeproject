#ifndef XTOOLS_H_
#define XTOOLS_H_

#include "X11/Xlib.h"
#include "X11/Xutil.h"
#include "X11/Xos.h"
#include <X11/Xresource.h>
#include "xwindow.h"
#include "xbutton.h"
#include "xframe.h"
#include "xlabel.h"
#include "xtextbox.h"
#include "xdialog.h"
#include "xdraw.h"
#define DefGC(dpy) DefaultGC(dpy, DefaultScreen(dpy))
#define POINTERTOINT(a) ((int)(a))
#define INTTOPOINTER(a) ((XPointer)(a))


void x_init();
void x_main();
void x_main_quit();
void x_clear_modal();
void x_handle_event(XEvent event);

#endif
