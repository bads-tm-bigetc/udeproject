/* 1999 by Jaime ALberto Silva Colorado and Christian Ruppert, licensed
   under GPL */


/*  TODOS:

  * add support for radio lists (UDEButtonRadio flag)
  * add a destructor and a modifier function.
  * save wether the hex shape was created by the constructor to free it in the
    destructor

*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#ifdef HAVE_XSHAPEQUERYEXTENSION
#include <X11/extensions/shape.h>
#endif

#include "ude-desktop.h"
#include "ude-box.h"
#include "ude-hex.h"
#include "ude-button.h"
#include "ude-events.h"
#include "ude-bevels.h"
#include "ude-internals.h"

/*************************** internal stuff *******************************/

char _UDE_button_actually_inside = 0;
static int _UDE_button_actually_pressed_buttons = 0;

#define UDE_BUTTON_POINTER_MASK (EnterWindowMask | LeaveWindowMask |\
                                 ButtonPressMask | ButtonReleaseMask)

void _UDE_refresh_button(UDEBox *box, UDEPointer data, XEvent *event)
{
  int invert;
  invert = (((UDEButton *)box)->flags & UDEButtonSwitch) 
           && (((UDEButton *)box)->flags & UDEButtonDown)
           && _UDE_button_actually_pressed_buttons;
  if(((UDEButton *)box)->flags & UDEButtonUserPixmaps) {
    switch (((UDEButton *)box)->display_status){
      case UDEButtonDispPassive:
        if(!(((UDEButton *)box)->flags & UDEButtonSwitch)){
          XSetWindowBackgroundPixmap(box->desktop->disp, box->win,
                                     ((UDEButton *)box)->std);
          break;
        }
      case UDEButtonDispMouseOver:
        if((!(((UDEButton *)box)->flags & UDEButtonSwitch))
           || (!(((UDEButton *)box)->flags & UDEButtonDown))
           || (_UDE_button_actually_pressed_buttons)){
          XSetWindowBackgroundPixmap(box->desktop->disp, box->win,
                                     invert ? ((UDEButton *)box)->active
                                     : ((UDEButton *)box)->mouse_over);
          break;
        }
      case UDEButtonDispActive:
        XSetWindowBackgroundPixmap(box->desktop->disp, box->win,
                                   invert ? ((UDEButton *)box)->mouse_over
                                   : ((UDEButton *)box)->active);
        break;
    }
    XClearWindow (box->desktop->disp, box->win);
  } else {
    GC SC, LC;
    XGCValues xgcv;
    int w,h;

    xgcv.function = GXcopy;
    xgcv.foreground = box->desktop->ActualColors[invert ? UDE_Shadow
                                                 : UDE_Light].pixel;
    xgcv.line_width = 0;
    xgcv.line_style = LineSolid;
    xgcv.cap_style = CapButt;
    LC = XCreateGC(box->desktop->disp, box->win, GCFunction|GCForeground|\
                   GCCapStyle|GCLineWidth|GCLineStyle, &xgcv);
    xgcv.function = GXcopy;
    xgcv.foreground = box->desktop->ActualColors[invert ? UDE_Light
                                                 : UDE_Shadow].pixel;
    xgcv.line_width = 0;
    xgcv.line_style = LineSolid;
    xgcv.cap_style = CapButt;
    SC = XCreateGC(box->desktop->disp, box->win, GCFunction|GCForeground|\
                   GCCapStyle|GCLineWidth|GCLineStyle, &xgcv);

    XClearWindow (box->desktop->disp, box->win);

    switch (((UDEButton *)box)->display_status){
      case UDEButtonDispPassive:
        if(!(((UDEButton *)box)->flags & UDEButtonSwitch)){
#ifdef HAVE_XSHAPEQUERYEXTENSION
          if(((UDEButton *)box)->flags & UDEButtonShaped)
            ude_draw_hex_groove(box, 0, 0, box->attributes.width,
                                box->attributes.height,
                                box->desktop->BevelWidth, SC, LC);
          else
#endif
            ude_draw_groove(box, 0, 0, box->attributes.width - 1,
                            box->attributes.height - 1,
                            box->desktop->BevelWidth, SC, LC);
          break;
        }
      case UDEButtonDispMouseOver:
        if((!(((UDEButton *)box)->flags & UDEButtonSwitch))
           || (!(((UDEButton *)box)->flags & UDEButtonDown))
           || (_UDE_button_actually_pressed_buttons)){
#ifdef HAVE_XSHAPEQUERYEXTENSION
          if(((UDEButton *)box)->flags & UDEButtonShaped)
            ude_draw_hex_bevel(box, 0, 0, box->attributes.width,
                               box->attributes.height,
                               box->desktop->BevelWidth, LC, SC);
          else
#endif
            ude_draw_bevel(box, 0, 0, box->attributes.width - 1,
                           box->attributes.height - 1,
                           box->desktop->BevelWidth, LC, SC);
          break;
        }
      case UDEButtonDispActive:
#ifdef HAVE_XSHAPEQUERYEXTENSION
        if(((UDEButton *)box)->flags & UDEButtonShaped)
          ude_draw_hex_bevel(box, 0, 0, box->attributes.width,
                             box->attributes.height,
                             box->desktop->BevelWidth, SC, LC);
        else
#endif
          ude_draw_bevel(box, 0, 0, box->attributes.width - 1,
                         box->attributes.height - 1,
                         box->desktop->BevelWidth, SC, LC);
        break;
    }

    XFreeGC (box->desktop->disp, SC);
    XFreeGC (box->desktop->disp, LC);

    if(strlen(((UDEButton *)box)->name)){
      GC TC;
      xgcv.function = GXcopy;
      xgcv.foreground = box->desktop->ActualColors[UDE_StandardText].pixel;
      xgcv.font=box->desktop->StandardFontStruct->fid;
      TC = XCreateGC (box->desktop->disp, box->win,
                      GCFunction|GCForeground|GCFont, &xgcv);

      w = XTextWidth(box->desktop->StandardFontStruct,
                     ((UDEButton *)box)->name,
                     strlen(((UDEButton *)box)->name));
      h = box->desktop->StandardFontStruct->ascent
          + box->desktop->StandardFontStruct->descent;
      XDrawString(box->desktop->disp, box->win, TC,
                  (box->attributes.width - w)/2, (box->attributes.height - h)/2 
                  + box->desktop->StandardFontStruct->ascent,
                  ((UDEButton *)box)->name, strlen(((UDEButton *)box)->name));
      XFreeGC (box->desktop->disp, TC);
    }
  }
  if(!(((UDEButton *)box)->flags & UDEButtonSelectable)){
    GC SC;
    XGCValues xgcv;

    xgcv.function = GXcopy;
    xgcv.foreground = box->desktop->ActualColors[UDE_Shadow].pixel;
    xgcv.cap_style = CapButt;
    xgcv.line_width = 0;
    xgcv.line_style = LineSolid;
    xgcv.fill_style = FillStippled;
    xgcv.stipple = box->desktop->internal.raster_pixmap;
    SC = XCreateGC(box->desktop->disp, box->win, GCFunction|GCForeground|\
                   GCCapStyle|GCLineWidth|GCLineStyle|GCFillStyle|GCStipple,
                   &xgcv);
    XFillRectangle(box->desktop->disp, box->win, SC, 0, 0,
                   box->attributes.width, box->attributes.height);
    XFreeGC (box->desktop->disp, SC);
  }
}

inline void
_UDE_set_button_display_status(UDEButton *button, unsigned char status)
{
  button->display_status = status;
  _UDE_refresh_button(&button->box, NULL, NULL);
}

void
_UDE_button_press_handler(UDEBox *box, UDEPointer data, XEvent *event)
{
  if(!(((UDEButton *)box)->flags & UDEButtonSelectable)){
    if(_UDE_button_actually_pressed_buttons) {
      _UDE_button_actually_pressed_buttons = 0;
      _UDE_set_button_display_status((UDEButton *)box, UDEButtonDispPassive);
    }
    return;
  }
  _UDE_button_actually_pressed_buttons++;
  if(_UDE_button_actually_pressed_buttons == 1)
  {
    ude_grab_pointer(box->desktop, box->win, False, UDE_BUTTON_POINTER_MASK,
                     GrabModeAsync, GrabModeAsync, None, None);

    if(!(((UDEButton *)box)->flags & UDEButtonSwitch)){
      ((UDEButton *)box)->flags |= UDEButtonDown;
    }

    _UDE_set_button_display_status((UDEButton *)box, UDEButtonDispActive);
    ude_send_ude_event (box, UDEETouched);
  }
}

void
_UDE_button_release_handler(UDEBox *box, UDEPointer data, XEvent *event)
{
  if(!(((UDEButton *)box)->flags & UDEButtonSelectable)){
    if(_UDE_button_actually_pressed_buttons) {
      _UDE_button_actually_pressed_buttons = 0;
      _UDE_set_button_display_status((UDEButton *)box, UDEButtonDispPassive);
    }
    return;
  }
  _UDE_button_actually_pressed_buttons--;
  if(_UDE_button_actually_pressed_buttons) return;
  ude_ungrab_pointer(box->desktop);
  if(_UDE_button_actually_inside)
  {
    if(!(((UDEButton *)box)->flags & UDEButtonSwitch)){
      ((UDEButton *)box)->flags &= ~UDEButtonDown;
    } else {
      ((UDEButton *)box)->flags ^= UDEButtonDown;
    }
    _UDE_set_button_display_status((UDEButton *)box, UDEButtonDispMouseOver);
    ude_send_ude_event (box, UDEEAction);
  } else {
    _UDE_set_button_display_status((UDEButton *)box, UDEButtonDispPassive);
  }
}

void
_UDE_button_enter_handler(UDEBox *box, UDEPointer data, XEvent *event)
{
  if(!(((UDEButton *)box)->flags & UDEButtonSelectable)) return;
  _UDE_set_button_display_status((UDEButton *)box,
                                 _UDE_button_actually_pressed_buttons
                                 ? UDEButtonDispActive 
                                 : UDEButtonDispMouseOver);

  if((!(((UDEButton *)box)->flags & UDEButtonSwitch)) &&
     _UDE_button_actually_pressed_buttons){
    ((UDEButton *)box)->flags |= UDEButtonDown;
  }

  _UDE_button_actually_inside = -1;
}

void
_UDE_button_leave_handler(UDEBox *box, UDEPointer data, XEvent *event)
{
  if(!(((UDEButton *)box)->flags & UDEButtonSelectable)) return;
  _UDE_set_button_display_status((UDEButton *)box,
                                 _UDE_button_actually_pressed_buttons
                                 ? UDEButtonDispMouseOver 
                                 : UDEButtonDispPassive);

  if((!(((UDEButton *)box)->flags & UDEButtonSwitch)) &&
     _UDE_button_actually_pressed_buttons){
    ((UDEButton *)box)->flags &= ~UDEButtonDown;
  }

  _UDE_button_actually_inside = 0;
}

/*************************** end of internal stuff *************************/

UDEButton*
ude_button_create (UDEDesktop *desk, UDEBox *parent, char *name,
                   int x, int y, unsigned int width, unsigned int height,
                   Pixmap std, Pixmap mouse_over, Pixmap active, Pixmap shape,
                   unsigned int flags, UDEButton *radio, UDEPointer UserData)
{
  XSetWindowAttributes xswa;
  unsigned long valuemask;
  UDEButton *button;

  valuemask = ((flags & UDEButtonUserPixmaps) ? CWBackPixmap : CWBackPixel)
              | CWEventMask;
  xswa.background_pixmap = std;
  xswa.background_pixel = desk->ActualColors[UDE_Back].pixel;
  xswa.event_mask = UDE_BUTTON_POINTER_MASK | ExposureMask;

  button = (UDEButton *) ude_box_create (desk, parent, x, y, width, height, 0,
                                         CopyFromParent, CopyFromParent, &xswa,
                                         valuemask, UserData);
  if(!button) return(NULL);
  button = (UDEButton*) realloc (button, sizeof (UDEButton));
  if(!button) return(NULL);
  XSaveContext (button->box.desktop->disp, button->box.win, UDEContext,
                    (XPointer)button);

  button->flags = flags;
  button->std = std;
  button->mouse_over = mouse_over;
  button->active = active;
  button->name = name;
  button->display_status = UDEButtonDispPassive;
  if(radio && (flags & UDEButtonRadio)){
    button->nextradio = radio;
    button->prevradio = button->nextradio->prevradio;
    if (button->prevradio)
      button->prevradio->nextradio = button;
    button->nextradio->prevradio = button;
  } else {
    button->prevradio = button->nextradio = NULL;
  }
#ifdef HAVE_XSHAPEQUERYEXTENSION
  if(flags & UDEButtonShaped) {
    if(shape == None) {
      button->shape = ude_create_hex_shape(desk, width, height);
    } else {
      button->shape = shape;
    }
    XShapeCombineMask(desk->disp, button->box.win, ShapeBounding, 0, 0,
                      button->shape, ShapeSet);
  }
#else
  button->shape = None;
#endif
  ude_add_event_handler (&button->box, XEVENTS_GROUP, Expose,
                         _UDE_refresh_button, button->box.UserData);
  ude_add_event_handler (&button->box, XEVENTS_GROUP, ButtonPress,
                         _UDE_button_press_handler, button->box.UserData);
  ude_add_event_handler (&button->box, XEVENTS_GROUP, ButtonRelease,
                         _UDE_button_release_handler, button->box.UserData);
  ude_add_event_handler (&button->box, XEVENTS_GROUP, EnterNotify,
                         _UDE_button_enter_handler, button->box.UserData);
  ude_add_event_handler (&button->box, XEVENTS_GROUP, LeaveNotify,
                         _UDE_button_leave_handler, button->box.UserData);
  ude_add_x_events_to_mask (&button->box, ExposureMask | ButtonPressMask
                                          | ButtonReleaseMask 
                                          | EnterWindowMask | LeaveWindowMask);
  return(button);
}
