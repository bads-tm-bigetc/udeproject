/*** UDE-DESKTOP.H: contains all predefinitions related to a ude desktop.  ***/

/* ########################################################################

   libUDE - the uNIX dESKTOP eNVIRONMENT application library.

   ########################################################################

   Copyright (c) : Christian Ruppert

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   ######################################################################## */

/* this file will be expanded in future */

#ifndef _UDE_DISPLAY_H
#define _UDE_DISPLAY_H

#include <X11/Xlib.h>

#include "ude-internals.h"

extern Atom UDEContext, UDE_WORKSPACES_PROPERTY, UDE_SETTINGS_PROPERTY,
            UDE_WINDOW_PROPERTY;

#define UDE_MAXCOLORS 10
#define UDE_Back 0
#define UDE_Light 1
#define UDE_Shadow 2
#define UDE_StandardText 3
#define UDE_InactiveText 4
#define UDE_InactiveBgr 5
#define UDE_HighlightedText 6
#define UDE_HighlightedBgr 7
#define UDE_TextColor 8
#define UDE_TextBgr 9
typedef XColor UDEColor;
typedef UDEColor UDEColors[UDE_MAXCOLORS];

/*** structure available for each window in window's UDE_WINDOW_PROPERTY. ***/
#define UDE_STICKYWIN (-1)
typedef struct _UDEWin UDEWin;

struct _UDEWin {
  Display *disp;        /* guess we need this to identify the win and colors */
  Window win;
  short WorkSpace;
  /* ... */
};

/*** global array of structure available in root win's UDE_WORKSPACES_PROPERTY

           THIS IS BUGGY!!! REEMPLEMENTATION NEEDED!!!

typedef struct _UDEWorkspaceExchange UDEWorkspaceExchange;

struct _UDEWorkspaceExchange {
  char name[32];
  UDEColors WorkspaceColors;
};
                                                                            ***/

/*** global structure, available only once in root win's UDE_SETTINGS_PROPERTY.
                                                                            ***/
/** user preferences flags (item flags): **/
#define UDETransientMenus (1<<0) /* are menues transient or not? */
#define UDESubMenuTitles  (1<<1) /* do submenus have titles or not? */
typedef struct _UDEDesktop UDEDesktop;

struct _UDEDesktop { /* this is also buggy and needs reimplementation */
  Display *disp;        /* we need this to identify the gcs and fonts */
  unsigned short flags;
  unsigned int BevelWidth;
/*  UDEWorkspaceExchange *WorkSpacesData;  buggy, needs reimplementation */
  short ActiveWorkSpace, WorkSpaces;
  UDEColor *ActualColors;    /* preallocated color cells to be used with ALL
                                ude colors drawing and refreshing! */
  char StandardFont[256], InactiveFont[256], HighlightFont[256],
       TextFont[256];
     /* font names to be passed to XLoadQueryFont() */
  XFontStruct *StandardFontStruct, *InactiveFontStruct, *HighlightFontStruct,
              *TextFontStruct;
  struct {         /* characteristics of the system's shapes extension */
    int event_base;
    int error_base;
  } shape_ext;
  struct {         /* library internal stuff, leave untouched */
    int sgrabstat;
    _UDE_INTERNAL_pgrabstat *pgrabstat;
    Pixmap raster_pixmap;
  } internal;
  /* ... */
};

char ude_initialize (UDEDesktop *desktop);
void ude_error_exit(int ecode, char *message);

#endif
