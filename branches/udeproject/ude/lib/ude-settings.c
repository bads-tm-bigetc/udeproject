/*** SETTINGS.C: Contains routines to obtain and update data taken from
                 like desktop color settings etc. ***/

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <X11/Xlib.h>
#include "ude-desktop.h"
#include "ude-types.h"
#include "ude-box.h"

/******************* PRIVATE: DO NOT USE THIS EXTERNALLY ******************/

Atom  UDE_WORKSPACES_PROPERTY, UDE_SETTINGS_PROPERTY, UDE_WINDOW_PROPERTY;

UDEWorkspaceExchange _UDE_default_workspace = {"UDE",{
                              {0,150<<8,106<<8,75<<8,DoRed|DoGreen|DoBlue,0},
                              {0,225<<8,159<<8,113<<8,DoRed|DoGreen|DoBlue,0},
                              {0,100<<8,35<<8,50<<8,DoRed|DoGreen|DoBlue,0},
                              {0,255<<8,193<<8,37<<8,DoRed|DoGreen|DoBlue,0},
                              {0,150<<8,150<<8,150<<8,DoRed|DoGreen|DoBlue,0},
                              {0,255<<8,255<<8,255<<8,DoRed|DoGreen|DoBlue,0},
                              {0,255<<8,193<<8,37<<8,DoRed|DoGreen|DoBlue,0},
                              {0,63<<8,45<<8,31<<8,DoRed|DoGreen|DoBlue,0},
                              }};

/*** called at library initialisation to init the atoms. ***/
void
ude_init_settings(Display *disp)
{
  UDE_WORKSPACES_PROPERTY = XInternAtom(disp, "_UDE_WORKSPACES_PROPERTY",False);
  UDE_SETTINGS_PROPERTY = XInternAtom(disp, "_UDE_SETTINGS_PROPERTY", False);
  UDE_WINDOW_PROPERTY = XInternAtom(disp,"_UDE_WINDOW_PROPERTY", False);
}

inline XFontStruct*
ude_load_font(Display *disp, char *name,XFontStruct **font)
{
  if(*font) XFreeFont(disp,*font);
  *font = XLoadQueryFont(disp,name);
  if(!(*font)) *font = XLoadQueryFont(disp,"fixed");
}

/***************************************************************************/

/* will update the UDEDesktop structure given as argument's WorkSpacesData 
   element.
   return values: UDE_SUCCESS   in case all obtained values have been valid 
                  UDE_BADVAL    in case an error occured */
char
ude_update_UDEWorkspaceExchange(UDEDesktop *desk)
{
  Atom atype;
  int aformat,success;
  unsigned long n,ba;

  success = XGetWindowProperty(desk->disp,
                     RootWindow(desk->disp,DefaultScreen (desk->disp)),
                     UDE_WORKSPACES_PROPERTY, 0,
                     sizeof(UDEWorkspaceExchange) * desk->WorkSpaces,
                     False, UDE_WORKSPACES_PROPERTY, &atype, &aformat, &n, &ba,
                     (unsigned char **) &desk->WorkSpacesData);
  if((aformat != 8) || (n < (sizeof(UDEWorkspaceExchange) * desk->WorkSpaces))
     || (atype != UDE_WORKSPACES_PROPERTY) || (success != Success)){
    /* something went wrong... mark pointer as invalid and return error */
    desk->WorkSpacesData = &_UDE_default_workspace;
    return(UDE_BADVAL);
  } else {
    /* received valid data: return success */
    return(UDE_SUCCESS);
  }
}

/* will update the UDEDesktop structure given as argument.
   return values: UDE_SUCCESS   in case all obtained values have been valid 
                  UDE_BADVAL    in case an error occured 
   regardless of the return value the argument structure is initialized with
   valid values. */
char
ude_update_UDEDesktop(UDEDesktop *desk)
{
  Atom atype;
  int aformat,success;
  unsigned long n,ba;
  UDEDesktop *data;
  char ret;

  success = XGetWindowProperty(desk->disp,
                     RootWindow(desk->disp,DefaultScreen (desk->disp)),
                     UDE_SETTINGS_PROPERTY, 0, sizeof(UDEDesktop), False,
                     UDE_SETTINGS_PROPERTY, &atype, &aformat, &n, &ba,
                     (unsigned char **) &data);
  if((aformat != 8) || (n < sizeof(UDEDesktop))
     || (atype != UDE_SETTINGS_PROPERTY) || (success != Success)){
    /* something went wrong: set desktop structure to defaults */
    int a;

    if(desk->ActualColors) {
      unsigned long pixels[UDE_MAXCOLORS];
      for(a=0;a<UDE_MAXCOLORS;a++)
        pixels[a] = desk->ActualColors[a].pixel;
      XFreeColors(desk->disp,
                  DefaultColormap(desk->disp,DefaultScreen(desk->disp)),
                  pixels, UDE_MAXCOLORS, 0);
    }
    desk->WorkSpacesData = &_UDE_default_workspace;
    desk->ActiveWorkSpace = 0;
    desk->WorkSpaces = 1;
    desk->ActualColors = desk->WorkSpacesData
                               [desk->ActiveWorkSpace].WorkspaceColors;
    for(a=0;a<UDE_MAXCOLORS;a++)
      XAllocColor(desk->disp,
                  DefaultColormap(desk->disp, DefaultScreen(desk->disp)),
                  &desk->ActualColors[a]);
    
    desk->BevelWidth = 2;
    desk->flags = UDETransientMenus;
    
    sprintf(desk->StandardFont, "lucidasans-12");
    sprintf(desk->HighlightFont, "lucidasans-12");
    sprintf(desk->InactiveFont, "lucidasans-12");
    sprintf(desk->TextFont, "lucidasans-12");
    
    ret = UDE_BADVAL;
  } else {
    /* received valid data, copy to desktop structure. */

    ret = UDE_SUCCESS;

    if(((desk->ActiveWorkSpace != data->ActiveWorkSpace) 
       || (desk->WorkSpaces != data->WorkSpaces)) && (desk->ActualColors)) {
      int a;
      unsigned long pixels[UDE_MAXCOLORS];
      for(a=0;a<UDE_MAXCOLORS;a++)
        pixels[a] = desk->ActualColors[a].pixel;
      XFreeColors(desk->disp,
                  DefaultColormap(desk->disp,DefaultScreen(desk->disp)),
                  pixels, UDE_MAXCOLORS, 0);
    }

    if(desk->WorkSpaces != data->WorkSpaces){
      desk->WorkSpaces = data->WorkSpaces;
      ret = ude_update_UDEWorkspaceExchange(desk);
    }

    if((desk->ActiveWorkSpace != data->ActiveWorkSpace)
       || (desk->WorkSpaces != data->WorkSpaces)) {
      int a;
      desk->ActiveWorkSpace = data->ActiveWorkSpace;
      desk->ActualColors = desk->WorkSpacesData
                                 [desk->ActiveWorkSpace].WorkspaceColors;
      for(a=0;a<UDE_MAXCOLORS;a++)
        XAllocColor(desk->disp,
                    DefaultColormap(desk->disp, DefaultScreen(desk->disp)),
                    &desk->ActualColors[a]);
    }

    strncpy(desk->StandardFont, data->StandardFont, 255);
    strncpy(desk->HighlightFont, data->HighlightFont, 255);
    strncpy(desk->InactiveFont, data->InactiveFont, 255);
    strncpy(desk->TextFont, data->TextFont, 255);
    desk->BevelWidth = data->BevelWidth;
    desk->flags = data->flags;
  }

  ude_load_font(desk->disp,desk->StandardFont,&desk->StandardFontStruct);
  ude_load_font(desk->disp,desk->HighlightFont,&desk->HighlightFontStruct);
  ude_load_font(desk->disp,desk->InactiveFont,&desk->InactiveFontStruct);
  ude_load_font(desk->disp,desk->TextFont,&desk->TextFontStruct);

  return(ret);
}

/* will determine the box's workspace.
   return values: UDE_STICKYWIN in case window is sticky 
                  UDE_BADVAL    in case an error occured
                  any number>=0 as workspace number
   ATTENTION: don't check for UDE_FAIL or UDE_SUCCESS, corresponding values are
   reserved for workspace 0 and sticky windows. */
short
ude_get_box_workspace(UDEBox *box)
{
  Atom atype;
  int aformat,success;
  unsigned long n,ba;

  UDEWin *win;

  while(box->parent) box=box->parent;

  success = XGetWindowProperty(box->desktop->disp, box->win,
                     UDE_WINDOW_PROPERTY, 0, sizeof(UDEWin), False,
                     UDE_WINDOW_PROPERTY, &atype, &aformat, &n, &ba,
                     (unsigned char **) &win);
  if((aformat != 8) || (n < sizeof(UDEWin)) || (atype != UDE_WINDOW_PROPERTY)
     || (success != Success)){
    /* no or invalid structure received, return error */
    return(UDE_BADVAL);
  } else {  /* received data seems to be valid */
    return(win->WorkSpace);
  }
}
