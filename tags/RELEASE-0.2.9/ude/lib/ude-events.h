#ifndef _UDE_EVENTS_H
#define _UDE_EVENTS_H

/* Here will be declared all the ude events, each time some ude-lib programer
   needs a new event can define it here or can check for an already defined
   event name that can use.
   The events will be numbered from 1 to 2^32 - 1 because the event is
   represented by an integer. The zero (0) will not be used to allow
   event checking */
/* naming conventioin: all events defined here should start with UDEE
   for ude event to avoid collisions. */

#define UDEEAction (1)     /* sent in case operation on a widget is completed */
                           /* e.g. if a button is pressed and released again. */
#define UDEETouched (2)    /* sent as soon as a widget is touched by the user */
                           /* e.g. as soon as a button is pressed. */

#endif /* !_UDE_EVENTS_H */
