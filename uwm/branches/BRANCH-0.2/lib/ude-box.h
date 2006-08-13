#ifndef _UDE_BOX_H_
#define _UDE_BOX_H_

#include <X11/Xlib.h>
#include "ude-types.h"
#include "ude-vector.h"
#include "ude-desktop.h"

#define UDE_all_boxes_events_mask (StructureNotifyMask)

typedef struct _UDEBox UDEBox;

struct _UDEBox {
  UDEDesktop *desktop;                /* just in case the program operates on
                                         several displays. we could eventually
                                         overtake the UDEDesktop from the old
                                         desktop.h */
  struct _UDEBox *parent;             /* the parent box or NULL if the box
                                         represents a top level window */
  Window win;                         /* The Window representing the box on the
                                         screen */
  XWindowAttributes attributes;       /* contains the box's window's actual
                                         attributes such as size and position */
  UDEVector *x_handlers;              /* this will contain all handler routines
                                         for X events arriving to the
                                         corresponding window */
  UDEVector *ude_handlers;            /* this will contain all handler routines
                                         for UDE events arriving to the
                                         corresponding window */
  UDEPointer UserData;                /* for free use for the program that uses
                                         libUDE */
/* to be extended in future? */
};

UDEBox *ude_box_create (UDEDesktop *desk,
			UDEBox *parent,
			int x,
			int y, 
			unsigned int width,
			unsigned int height,
			unsigned int border_width,
			int depth,
			Visual *visual,
			XSetWindowAttributes *winattributes,
			unsigned long valuemask,
			UDEPointer UserData);

UDEBox *ude_simple_box_create (UDEDesktop *desk,
			       UDEBox *parent,
			       int x,
			       int y,
			       unsigned int width,
			       unsigned int height,
			       unsigned int border_width,
			       unsigned long border,
			       unsigned long background,
			       long event_mask,
			       UDEPointer UserData);

void ude_box_activate (UDEBox *box);
void ude_box_deactivate (UDEBox *box);
void ude_box_destroy (UDEBox *box);

/* event handling routines */
typedef void (*UDEEventHandlerFunction) (UDEBox *box,
					 UDEPointer data,
					 XEvent *event);
/* This type will be the event group passed ude_add_event_handler the events
   are divided in X events and UDE events */
typedef enum _UDEEventGroup UDEEventGroup;
enum _UDEEventGroup {
  XEVENTS_GROUP,
  UDEEVENTS_GROUP
};

void ude_add_event_handler (UDEBox *box,
			    UDEEventGroup event_group,
			    int event_type,
			    UDEEventHandlerFunction function,
			    UDEPointer data);
void ude_add_event_handler_first (UDEBox *box,
				  UDEEventGroup event_group,
				  int event_type,
				  UDEEventHandlerFunction function,
				  UDEPointer data);
void ude_remove_event_handler (UDEBox *box,
			       UDEEventGroup event_group,
			       int event_type,
			       UDEEventHandlerFunction function);
/* This function will be used to dispatch the ude events */
void ude_send_ude_event (UDEBox *box, int ude_event_type);
void ude_handle_event_loop (UDEDesktop *desk, int (*cont)());

#endif /* !_UDE_BOX_H_ */
