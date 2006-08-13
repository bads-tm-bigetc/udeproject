/* 1999 by Ricardo Lipas Augusto, licensed under GPL */

/* 
 * TODOs:
 * Make horizontal menus
 * Make recursive menus
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "ude-box.h"
#include "ude-menu.h"
#include "ude-desktop.h"
#include "ude-bevels.h"
#include "ude-events.h"
#include "ude-internals.h"

/*************************** internal stuff *******************************/
static int _UDE_menuitem_actually_pressed_buttons = 0;
char _UDE_menuitem_actually_inside = 0;

#define UDE_MENUITEM_POINTER_MASK (EnterWindowMask | LeaveWindowMask |\
		OwnerGrabButtonMask | ButtonPressMask | ButtonReleaseMask)

void _UDE_refresh_menu(UDEBox *box, UDEPointer data, XEvent *event)
{
	GC SC, LC, TC;
	XGCValues xgcv;

	xgcv.function = GXcopy;
	xgcv.foreground = box->desktop->ActualColors[UDE_Light].pixel;
	xgcv.line_width = 0;
	xgcv.line_style = LineSolid;
	xgcv.cap_style = CapButt;
	LC = XCreateGC(box->desktop->disp, box->win, GCFunction|GCForeground|
				GCCapStyle|GCLineWidth|GCLineStyle, &xgcv);
	xgcv.function = GXcopy;
	xgcv.foreground = box->desktop->ActualColors[UDE_Shadow].pixel;
	xgcv.line_width = 0;
	xgcv.line_style = LineSolid;
	xgcv.cap_style = CapButt;
	SC = XCreateGC(box->desktop->disp, box->win, GCFunction|GCForeground|
				GCCapStyle|GCLineWidth|GCLineStyle, &xgcv);

	XClearWindow (box->desktop->disp, box->win);

	ude_draw_bevel(box, 0, 0, box->attributes.width - 1,
		box->attributes.height - 1, box->desktop->BevelWidth, LC, SC);
	ude_draw_bevel(box, 2, 2, box->attributes.width - 3,
		((UDEMenu *)box)->itemheight+1, box->desktop->BevelWidth,SC,LC);

	XFreeGC (box->desktop->disp, SC);
	XFreeGC (box->desktop->disp, LC);

	xgcv.function = GXcopy;
	xgcv.foreground = box->desktop->ActualColors[UDE_StandardText].pixel;
	xgcv.font=box->desktop->StandardFontStruct->fid;
	TC = XCreateGC (box->desktop->disp, box->win,
					GCFunction|GCForeground|GCFont, &xgcv);
	XDrawString(box->desktop->disp, box->win, TC, 4+4, 
		2+4 + box->desktop->StandardFontStruct->ascent,
		((UDEMenu *)box)->name, strlen(((UDEMenu *)box)->name));

	return;
}

void _UDE_refresh_menuitem(UDEBox *box, UDEPointer data, XEvent *event)
{
        GC SC, LC, TC;
        XGCValues xgcv;

        xgcv.function = GXcopy;
        xgcv.foreground = box->desktop->ActualColors[UDE_Light].pixel;
        xgcv.line_width = 0;
        xgcv.line_style = LineSolid;
        xgcv.cap_style = CapButt;
        LC = XCreateGC(box->desktop->disp, box->win, GCFunction|GCForeground|
                                GCCapStyle|GCLineWidth|GCLineStyle, &xgcv);
        xgcv.function = GXcopy;
        xgcv.foreground = box->desktop->ActualColors[UDE_Shadow].pixel;
        xgcv.line_width = 0;
        xgcv.line_style = LineSolid;
        xgcv.cap_style = CapButt;
        SC = XCreateGC(box->desktop->disp, box->win, GCFunction|GCForeground|
                                GCCapStyle|GCLineWidth|GCLineStyle, &xgcv);

        XClearWindow(box->desktop->disp, box->win);

        switch(((UDEMenuItem *)box)->display_status) {
        	case UDEMenuItemDispPassive:
        		if(!(((UDEMenuItem *)box)->flags & UDEMenuItemSwitch))
        			break;
        		if(((UDEMenuItem *)box)->flags & UDEMenuItemDown) {
				 ude_draw_bevel(box, 0, 0,
				 	box->attributes.width - 1,
				 	box->attributes.height - 1,
				 	1, SC, LC);
			}
			else {
				ude_draw_bevel(box, 0, 0,
					box->attributes.width - 1,
					box->attributes.height - 1,
					1, LC, SC);
			}
			break;

		case UDEMenuItemDispMouseOver:
			if(!(((UDEMenuItem *)box)->flags & UDEMenuItemSwitch)) {
				ude_draw_groove(box,0,0,box->attributes.width-1,
					box->attributes.height - 1,
					box->desktop->BevelWidth, SC, LC);
				break;
			}
			if(((UDEMenuItem *)box)->flags & UDEMenuItemDown) {
				ude_draw_groove(box,0,0,box->attributes.width-1,
					box->attributes.height - 1,
					box->desktop->BevelWidth, SC, LC);
			}
			else {
				ude_draw_groove(box,0,0,box->attributes.width-1,
					box->attributes.height - 1,
					box->desktop->BevelWidth, LC, SC);
			}
			break;

		case UDEMenuItemDispActive:
			if(!(((UDEMenuItem *)box)->flags & UDEMenuItemSwitch)) {
				ude_draw_bevel(box, 0, 0,
					box->attributes.width - 1,
					box->attributes.height - 1,
					box->desktop->BevelWidth, SC, LC);
				break;
			}
			if(((UDEMenuItem *)box)->flags & UDEMenuItemDown) {
				ude_draw_bevel(box, 0, 0,
					box->attributes.width - 1,
					box->attributes.height - 1,
					1, LC, SC);
			}
			else {
				ude_draw_bevel(box, 0, 0,
					box->attributes.width - 1,
					box->attributes.height - 1,
					1, SC, LC);
			}
			break;
	}

	XFreeGC(box->desktop->disp, SC);
	XFreeGC(box->desktop->disp, LC);

	xgcv.function = GXcopy;
	xgcv.foreground = box->desktop->ActualColors[UDE_StandardText].pixel;
	xgcv.font=box->desktop->StandardFontStruct->fid;
	TC = XCreateGC (box->desktop->disp, box->win,
					GCFunction|GCForeground|GCFont, &xgcv);
	XDrawString(box->desktop->disp, box->win, TC, 4+2,
				2+2 + box->desktop->StandardFontStruct->ascent,
					((UDEMenuItem *)box)->name,
					strlen(((UDEMenuItem *)box)->name));

	if(!(((UDEMenuItem *)box)->flags & UDEMenuItemSelectable)) {
		xgcv.function = GXcopy;
		xgcv.foreground = box->desktop->ActualColors[UDE_Shadow].pixel;
		xgcv.cap_style = CapButt;
		xgcv.line_width = 0;
		xgcv.line_style = LineSolid;
		xgcv.fill_style = FillStippled;
		xgcv.stipple = box->desktop->internal.raster_pixmap;
		SC = XCreateGC(box->desktop->disp, box->win, GCFunction|
				GCForeground|GCCapStyle|GCLineWidth|GCLineStyle|
				GCFillStyle|GCStipple,&xgcv);
		XFillRectangle(box->desktop->disp, box->win, SC, 0, 0,
				box->attributes.width, box->attributes.height);
		XFreeGC (box->desktop->disp, SC);
	}

        return;
}

inline void _UDE_set_menuitem_display_status(UDEMenuItem *menuitem,
							unsigned char status)
{
	menuitem->display_status = status;
	_UDE_refresh_menuitem(&(menuitem->box), NULL, NULL);
}

void _UDE_menuitem_press_handler(UDEBox *box, UDEPointer data, XEvent *event)
{
	if(!(((UDEMenuItem *)box)->flags & UDEMenuItemSelectable)) return;
	_UDE_menuitem_actually_pressed_buttons++;
	if(_UDE_menuitem_actually_pressed_buttons == 1) {
		_UDE_set_menuitem_display_status((UDEMenuItem *)box,
						UDEMenuItemDispActive);
	ude_send_ude_event(box, UDEETouched);
	}

	return;
}

void _UDE_menuitem_release_handler(UDEBox *box, UDEPointer data, XEvent *event)
{
	if(!(((UDEMenuItem *)box)->flags & UDEMenuItemSelectable)) return;
	_UDE_menuitem_actually_pressed_buttons--;
	if(_UDE_menuitem_actually_pressed_buttons) return;
	if(_UDE_menuitem_actually_inside)
	{
		if(((UDEMenuItem *)box)->flags & UDEMenuItemSwitch) {
			((UDEMenuItem *)box)->flags ^= UDEMenuItemDown;
		}
		_UDE_set_menuitem_display_status((UDEMenuItem *)box,
						UDEMenuItemDispMouseOver);
		ude_send_ude_event(box, UDEEAction);
	}
	else {
		_UDE_set_menuitem_display_status((UDEMenuItem *)box,
						UDEMenuItemDispPassive);
	}

	return;
}

void _UDE_menuitem_enter_handler(UDEBox *box, UDEPointer data, XEvent *event)
{
	if(!(((UDEMenuItem *)box)->flags & UDEMenuItemSelectable)) return;
	if(_UDE_menuitem_actually_pressed_buttons) {
		_UDE_set_menuitem_display_status((UDEMenuItem *)box,
						UDEMenuItemDispActive);
		ude_send_ude_event(box, UDEETouched);
	}
	else {
		_UDE_set_menuitem_display_status((UDEMenuItem *)box,
						UDEMenuItemDispMouseOver);
	}

	_UDE_menuitem_actually_inside = 1;
	
	return;
}

void _UDE_menuitem_leave_handler(UDEBox *box, UDEPointer data, XEvent *event)
{
	if(!(((UDEMenuItem *)box)->flags & UDEMenuItemSelectable)) return;
	_UDE_set_menuitem_display_status((UDEMenuItem *)box,
						UDEMenuItemDispPassive);
	_UDE_menuitem_actually_inside = 0;
	return;
}

/*************************** end of internal stuff *************************/

UDEMenu*
ude_menu_create(UDEDesktop *desk, UDEBox *parent, int x, int y, char *name,
		unsigned int flags, Pixmap std, UDEMenuItem **menuitems,
		UDEPointer UserData)
{
	XSetWindowAttributes xswa;
	unsigned long valuemask;
	UDEMenu *menu;
	unsigned int width, height, itemheight, i = 0, textwidth, numitems;

	itemheight = desk->StandardFontStruct->ascent
		+ desk->StandardFontStruct->descent + 2*2 + 2*2;
	width = XTextWidth(desk->StandardFontStruct, name, strlen(name));
	height = itemheight;
	while(menuitems[i] != NULL) {
		if(menuitems[i]->name != NULL) {
			textwidth = XTextWidth(desk->StandardFontStruct,
				menuitems[i]->name,strlen(menuitems[i]->name));
			if(width <= textwidth) width = textwidth;
		}
		height += itemheight;
		i++;
	}
	width += 4*2 + 2*4;
	height += 2*2;
	numitems = i+1;

	valuemask = CWBackPixel | CWEventMask;
	xswa.background_pixel = desk->ActualColors[UDE_Back].pixel;
	xswa.event_mask = ExposureMask;

	menu = (UDEMenu *)ude_box_create(desk, parent, x, y, width, height, 0,
		CopyFromParent, CopyFromParent, &xswa, valuemask, UserData);

	if(!menu) return(NULL);
	menu = (UDEMenu*)realloc(menu, sizeof(UDEMenu));
	if(!menu) return(NULL);
	XSaveContext(menu->box.desktop->disp, menu->box.win, UDEContext,
							(XPointer)menu);

	menu->flags = flags;
	if(name == NULL) menu->name = "Menu";
	else menu->name = name;
	menu->std = std;
	menu->itemheight = itemheight;
	menu->menu_item = (UDEMenuItem **)malloc(numitems *
							sizeof(UDEMenuItem *));
	if(menu->menu_item == NULL) {
	        free(menu);
		return(NULL);
	}
	for(i = 0; i < (numitems-1); i++) {
		valuemask = ((menuitems[i]->flags & UDEMenuItemUserPixmaps) ?
				CWBackPixmap : CWBackPixel) | CWEventMask; 
		xswa.background_pixmap = menuitems[i]->std;
		xswa.background_pixel = desk->ActualColors[UDE_Back].pixel;
		xswa.event_mask = UDE_MENUITEM_POINTER_MASK | ExposureMask;
		menu->menu_item[i] = (UDEMenuItem *)ude_box_create(desk,
				&(menu->box), 2, 2+((i+1)*itemheight), width-4,
				itemheight, 0, CopyFromParent, CopyFromParent,
						&xswa, valuemask, UserData);
		menu->menu_item[i] = (UDEMenuItem *)realloc(menu->menu_item[i],
							sizeof(UDEMenuItem));
		XSaveContext(menu->menu_item[i]->box.desktop->disp,
				menu->menu_item[i]->box.win, UDEContext,
					(XPointer)menu->menu_item[i]);
		menu->menu_item[i]->flags = menuitems[i]->flags;
		menu->menu_item[i]->display_status = UDEMenuItemDispPassive;
		menu->menu_item[i]->name = menuitems[i]->name;
		menu->menu_item[i]->std = menuitems[i]->std;
		menu->menu_item[i]->mouse_over = menuitems[i]->mouse_over;
		menu->menu_item[i]->active = menuitems[i]->active;
		ude_add_event_handler(&(menu->menu_item[i]->box), XEVENTS_GROUP,
			Expose, _UDE_refresh_menuitem, UserData);
		ude_add_event_handler(&(menu->menu_item[i]->box), XEVENTS_GROUP,
			ButtonPress, _UDE_menuitem_press_handler, UserData);
		ude_add_event_handler(&(menu->menu_item[i]->box), XEVENTS_GROUP,
			ButtonRelease, _UDE_menuitem_release_handler, UserData);
		ude_add_event_handler(&(menu->menu_item[i]->box), XEVENTS_GROUP,
			EnterNotify, _UDE_menuitem_enter_handler, UserData);
		ude_add_event_handler(&(menu->menu_item[i]->box), XEVENTS_GROUP,
			LeaveNotify, _UDE_menuitem_leave_handler, UserData);
		ude_add_x_events_to_mask(&(menu->menu_item[i]->box), 
			ExposureMask | ButtonPressMask | ButtonReleaseMask |
			EnterWindowMask | LeaveWindowMask);
		ude_box_activate(&(menu->menu_item[i]->box));
	}
	menu->menu_item[numitems-1] = NULL;

	ude_add_event_handler(&(menu->box), XEVENTS_GROUP, Expose,
				_UDE_refresh_menu, UserData);
	ude_add_x_events_to_mask(&(menu->box), ExposureMask);

	return(menu);
}

void
ude_menu_destroy(UDEMenu *menu)
{
	int i = 0;
	if(menu == NULL) return;
	while(menu->menu_item[i] != NULL) {
		ude_box_destroy(&(menu->menu_item[i]->box));
		i++;
	}
	free(menu->menu_item);
	ude_box_destroy((UDEBox *)menu);

	return;
}
