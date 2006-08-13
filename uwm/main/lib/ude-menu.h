#ifndef UDE_MENU_H
#define UDE_MENU_H

#include "ude-box.h"

/* UDEMenu flags */
#define UDEMenuUserPixmaps (1<<0)    /*Set if the given pixmaps are to be used*/
#define UDEMenuHorizontal (1<<1)     /*Is this going to be a horizontal menu?*/

/* UDEMenuItem flags */
#define UDEMenuItemDown (1<<0)
#define UDEMenuItemSelectable (1<<1)
#define UDEMenuItemSwitch (1<<2)
#define UDEMenuItemUserPixmaps (1<<3)
/* The flags above are very similar to the button flags */
#define UDEMenuItemPrevPixmaps (1<<4)	/*Use the previous item pixmaps?*/
#define UDEMenuItemLink2Menu (1<<5)	/*Is this item a link to another menu?*/
#define UDEMenuItemSeparator (1<<6)	/*Is this item a separator?*/
/* UDEMenuItem display_status */
#define UDEMenuItemDispPassive (0)
#define UDEMenuItemDispMouseOver (1)
#define UDEMenuItemDispActive (2)


typedef struct _UDEMenu UDEMenu;
typedef struct _UDEMenuItem UDEMenuItem;

struct _UDEMenuItem {
	UDEBox box;		/*Menu item box->You can leave it as it is
				if this is an argument to ude_menu_create*/
	unsigned int flags;		/*Flags*/
	unsigned char display_status;	/*Display status*/
	char *name;			/*Menu item name*/
	Pixmap std, mouse_over, active;	/*Menu item pixmap*/
};
            
struct _UDEMenu {
	UDEBox box;			/*Menu box*/
	unsigned int flags;		/*Flags*/
	char *name;			/*Menu name*/
	Pixmap std;			/*Menu name pixmap*/
	UDEMenuItem **menu_item;	/*This is a null-terminated list
					of pointers to UDEMenuItems*/
	int itemheight;
};

UDEMenu*
ude_menu_create (UDEDesktop *desk, UDEBox *parent, int x, int y, char *name,
                 unsigned int flags, Pixmap std, UDEMenuItem **menuitems,
                 UDEPointer UserData);
void
ude_menu_destroy (UDEMenu * menu);
#endif
