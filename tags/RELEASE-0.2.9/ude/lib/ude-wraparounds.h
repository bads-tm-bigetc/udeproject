#ifndef UDE_WRAPAROUNDS_H
#define UDE_WRAPAROUNDS_H

#include <X11/Xlib.h>

#include "ude-desktop.h"
#include "ude-box.h"

void ude_grab_pointer(UDEDesktop *desk, Window win, Bool owner_events,
                      unsigned int event_mask, int pointer_mode,
                      int keyboard_mode, Window confine_to, Cursor cursor);
void ude_ungrab_pointer(UDEDesktop *desk);
void ude_grab_server(UDEDesktop *desk);
void ude_ungrab_server(UDEDesktop *desk);
void ude_set_x_event_mask(UDEBox *box, long mask);
void ude_add_x_events_to_mask(UDEBox *box, long mask);

#endif
