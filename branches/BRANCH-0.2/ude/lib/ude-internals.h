#ifndef UDE_INTERNALS_H
#define UDE_INTERNALS_H

#include <X11/Xlib.h>

typedef struct __UDE_INTERNAL_pgrabstat _UDE_INTERNAL_pgrabstat;

struct __UDE_INTERNAL_pgrabstat {
  struct __UDE_INTERNAL_pgrabstat *next;
  Window win;
  Bool owner_events;
  unsigned int mask;
  int pointer_mode, keyboard_mode;
  Window confine_to;
  Cursor mouse;
};

#endif
