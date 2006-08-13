#ifndef UDE_BUTTON_H
#define UDE_BUTTON_H

#include "ude-box.h"

/* Buttons (hex and normal, also switches and radio button lists) */
/* UDEButton flags */
#define UDEButtonDown (1<<0) /* set if selected */
#define UDEButtonSelectable (1<<1) /* is button selectable or inactive? */
#define UDEButtonSwitch (1<<2) /* is button a switch or an action button? */
#define UDEButtonRadio (1<<3) /* does this button belong to a radio list? */
#define UDEButtonShaped (1<<4) /* is this button shaped? */
#define UDEButtonUserPixmaps (1<<5) /* Button will be automatically drawn using
          if not set. otherwise the given pixmaps are used. */
/* UDEButton display_status */
#define UDEButtonDispPassive (0)
#define UDEButtonDispMouseOver (1)
#define UDEButtonDispActive (2)

typedef struct _UDEButton UDEButton;

struct _UDEButton {
   UDEBox box;
   unsigned int flags;
   unsigned char display_status;
   Pixmap std, mouse_over, active, shape;
   char *name;
   struct _UDEButton *nextradio, *prevradio;
};

UDEButton*
ude_button_create (UDEDesktop *desk, UDEBox *parent, char *name,
                   int x, int y, unsigned int width, unsigned int height,
                   Pixmap std, Pixmap mouse_over, Pixmap active, Pixmap shape,
                   unsigned int flags, UDEButton *radio, UDEPointer UserData);

#endif
