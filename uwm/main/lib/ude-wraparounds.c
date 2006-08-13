#include <stdlib.h> 

#include "ude-wraparounds.h"
#include "ude-box.h"

void ude_grab_pointer(UDEDesktop *desk, Window win, Bool owner_events,
                      unsigned int event_mask, int pointer_mode,
                      int keyboard_mode, Window confine_to, Cursor cursor)
{
  _UDE_INTERNAL_pgrabstat *gs;
  
  gs = malloc (sizeof(_UDE_INTERNAL_pgrabstat));
  if (!gs) ude_error_exit(1,"FATAL: out of memory.");

  gs->next = desk->internal.pgrabstat;
  gs->win = win;
  gs->owner_events = owner_events;
  gs->mask = event_mask;
  gs->pointer_mode = pointer_mode;
  gs->keyboard_mode = keyboard_mode;
  gs->confine_to = confine_to;
  gs->mouse = cursor;
  while(XGrabPointer(desk->disp, gs->win, gs->owner_events, gs->mask,
                     gs->pointer_mode, gs->keyboard_mode, gs->confine_to,
		     gs->mouse, CurrentTime) != GrabSuccess);
  desk->internal.pgrabstat = gs;
}

void ude_ungrab_pointer(UDEDesktop *desk)
{
  _UDE_INTERNAL_pgrabstat *gs;

  gs = desk->internal.pgrabstat;
  if(!gs) ude_error_exit(1,"tried to ungrab ungrabbed pointer.");
  desk->internal.pgrabstat = gs->next;
  free(gs);

  gs = desk->internal.pgrabstat;
  if(!gs) XUngrabPointer(desk->disp,CurrentTime);
  else {
    XGrabPointer(desk->disp, gs->win, gs->owner_events, gs->mask,
                 gs->pointer_mode, gs->keyboard_mode, gs->confine_to,
                 gs->mouse, CurrentTime);
  }
}

void ude_grab_server(UDEDesktop *desk)
{
  if(!desk->internal.sgrabstat) XGrabServer(desk->disp);
  desk->internal.sgrabstat++;
}

void ude_ungrab_server(UDEDesktop *desk)
{
  desk->internal.sgrabstat--;
  if(desk->internal.sgrabstat < 0)
    ude_error_exit(1,"tried to ungrab ungrabbed server.");
  if(!desk->internal.sgrabstat) XUngrabServer(desk->disp);
}

void
ude_add_x_events_to_mask(UDEBox *box, long mask)
{
  XSelectInput (box->desktop->disp, box->win,
                box->attributes.your_event_mask | mask);
  XGetWindowAttributes (box->desktop->disp, box->win, &box->attributes);
}

void
ude_set_x_event_mask(UDEBox *box, long mask)
{
  XSelectInput (box->desktop->disp, box->win, mask | UDE_all_boxes_events_mask);
  XGetWindowAttributes (box->desktop->disp, box->win, &box->attributes);
}

