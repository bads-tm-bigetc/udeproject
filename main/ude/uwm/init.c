/***  INIT.C: Contains routines for the initialisation of uwm  ***/

/* ########################################################################

   uwm - THE ude WINDOW MANAGER

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


#define __USE_GNU

#ifdef HAVE_CONFIG_H
#include <config.h>
#else
#warning This will probably not compile without config.h
#endif

#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <signal.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#else
#error nop! it will not compile.
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/xpm.h>
#include <X11/keysym.h>
#include <X11/extensions/shape.h>

#include "nodes.h"

#include "uwm.h"
#include "special.h"
#include "init.h"
#include "menu.h"
#include "handlers.h"
#include "workspaces.h"
#include "uwmmenu.h"
#include "winmenumenu.h"
#include "urm.h"
#include "pix.h"
#include "MwmUtil.h"
#include "settings.h"
#include "confparse.h"
#include "uwm_intl.h"

extern char **environ;

extern UDEScreen TheScreen;
extern Display *disp;
extern XContext UWMContext;
extern XContext UWMGroupContext;
extern Atom WM_STATE_PROPERTY;
extern Atom WM_CHANGE_STATE;
extern Atom WM_TAKE_FOCUS;
extern Atom WM_DELETE_WINDOW;
extern Atom WM_PROTOCOLS;
extern Atom MOTIF_WM_HINTS;

extern InitStruct InitS;

void SetupCursors(void)
{
  TheScreen.Mice[C_DEFAULT] = XCreateFontCursor(disp,XC_X_cursor);
  TheScreen.Mice[C_N] = XCreateFontCursor(disp,XC_top_side);
  TheScreen.Mice[C_NO] = XCreateFontCursor(disp,XC_top_right_corner);
  TheScreen.Mice[C_O] = XCreateFontCursor(disp,XC_right_side);
  TheScreen.Mice[C_SO] = XCreateFontCursor(disp,XC_bottom_right_corner);
  TheScreen.Mice[C_S] = XCreateFontCursor(disp,XC_bottom_side);
  TheScreen.Mice[C_SW] = XCreateFontCursor(disp,XC_bottom_left_corner);
  TheScreen.Mice[C_W] = XCreateFontCursor(disp,XC_left_side);
  TheScreen.Mice[C_NW] = XCreateFontCursor(disp,XC_top_left_corner);
  TheScreen.Mice[C_DRAG] = XCreateFontCursor(disp,XC_fleur);
  TheScreen.Mice[C_BORDER] = XCreateFontCursor(disp,XC_center_ptr);
  TheScreen.Mice[C_WINDOW] = XCreateFontCursor(disp,XC_top_left_arrow);
}

const int iconpostab[ICONWINS][2] = {
  {0,14},
  {24,0},
  {24,56},
  {0,42},
  {48,14},
  {48,42},
  {72,28}
};

char *iconfiles[7] = {
  "iconify",
  "close",
  "autorise",
  "back",
  "kill",
  "menu",
  "really"
};

#include "shape.bitmap"
Pixmap DefaultHexShape;

void PrepareIcons()
{
  XpmAttributes xa;
  char *path, *filename;
  XSetWindowAttributes xswa;
  int a, b;

  DefaultHexShape = XCreateBitmapFromData(disp, TheScreen.root, shape_bits,
                                          shape_width, shape_height);
  if(DefaultHexShape == None) {
    SeeYa(1, "Problem generating default Hex icon shape.");
  }

  a = 0;
  if(settings.global_settings->HexPath) {
    path = MyCalloc(strlen(settings.global_settings->HexPath) + 15,
		    sizeof(char));
    strcpy(path, settings.global_settings->HexPath);
    filename = path + strlen(settings.global_settings->HexPath);
  } else {
    path = MyCalloc(strlen(TheScreen.udedir) + 19, sizeof(char));
    sprintf(path, "%sgfx", TheScreen.udedir);
    filename = path + strlen(TheScreen.udedir) + 3;
  }

  for(b = 0; b < 7; b++){
    Pixmap dummy;
    xa.valuemask = 0;
    sprintf(filename, "/%s.xpm", iconfiles[b]);
    a |= XpmReadFileToPixmap(disp, TheScreen.root, path,
			     &TheScreen.HexMenu.icons[b].IconPix,
			     &TheScreen.HexMenu.icons[b].IconShape, &xa);
    TheScreen.HexMenu.icons[b].x = - xa.x_hotspot;
    TheScreen.HexMenu.icons[b].y = - xa.y_hotspot;
    if(TheScreen.HexMenu.icons[b].IconShape == None) {
      TheScreen.HexMenu.icons[b].IconShape = DefaultHexShape;
      TheScreen.HexMenu.icons[b].x = iconpostab[b][0];
      TheScreen.HexMenu.icons[b].y = iconpostab[b][1];
    }
    xa.valuemask = 0;
    sprintf(filename, "/%ss.xpm", iconfiles[b]);
    a |= XpmReadFileToPixmap(disp, TheScreen.root, path,
			     &TheScreen.HexMenu.icons[b].IconSelectPix,
                             &dummy, &xa);
    if(dummy != None) XFreePixmap(disp, dummy);
  }

  free(path);

  if(a) {
    if(settings.global_settings->HexPath) {
      free(settings.global_settings->HexPath);
      settings.global_settings->HexPath = NULL;
      fprintf(TheScreen.errout, "UWM: icons specified in uwmrc not found, "
				"trying to load default.\n");
      PrepareIcons();
    } else {
      SeeYa(1,"Icon pixmaps could not be loaded");
    }
    return;
  }

  xswa.override_redirect = True;
  xswa.save_under = True;

  TheScreen.HexMenu.IconParent = XCreateWindow(disp, TheScreen.root, 0, 0,
					       104, 84, 0, CopyFromParent,
					       InputOutput, CopyFromParent,
					       (TheScreen.DoesSaveUnders
						 ? CWSaveUnder : 0)
					       | CWOverrideRedirect, &xswa);
  XSelectInput(disp, TheScreen.HexMenu.IconParent,
	       LeaveWindowMask | VisibilityChangeMask);
  XShapeCombineMask(disp, TheScreen.HexMenu.IconParent, ShapeBounding,
                    TheScreen.HexMenu.icons[0].x, TheScreen.HexMenu.icons[0].y,
                    TheScreen.HexMenu.icons[0].IconShape, ShapeSet);
  for(a = 0;a<ICONWINS;a++) {
    TheScreen.HexMenu.icons[a].IconWin = XCreateWindow(disp,
						TheScreen.HexMenu.IconParent,
						TheScreen.HexMenu.icons[a].x,
						TheScreen.HexMenu.icons[a].y,
						32, 28, 0, CopyFromParent,
						InputOutput, CopyFromParent,
						CWOverrideRedirect, &xswa);
    XSelectInput(disp, TheScreen.HexMenu.icons[a].IconWin, EnterWindowMask);
    XSetWindowBackgroundPixmap(disp, TheScreen.HexMenu.icons[a].IconWin,
                               TheScreen.HexMenu.icons[a].IconPix);
    XShapeCombineMask(disp, TheScreen.HexMenu.icons[a].IconWin, ShapeBounding,
                      0, 0, TheScreen.HexMenu.icons[a].IconShape, ShapeSet);
    XShapeCombineMask(disp, TheScreen.HexMenu.IconParent, ShapeBounding,
		      TheScreen.HexMenu.icons[a].x,
		      TheScreen.HexMenu.icons[a].y,
		      TheScreen.HexMenu.icons[a].IconShape, ShapeUnion);
  }
  XMapSubwindows(disp, TheScreen.HexMenu.IconParent);
}

unsigned long AllocColor(XColor *xcol)
{
  if(XAllocColor(disp, TheScreen.colormap, xcol))
    return(xcol->pixel);
  else {
    if((xcol->red + xcol->green + xcol->blue) < (384<<8)){
      fprintf(TheScreen.errout,\
              "UWM: Cannot alloc color: %d;%d;%d. Using BlackPixel.\n",
              xcol->red, xcol->green, xcol->blue);
      return(xcol->pixel = BlackPixel(disp,TheScreen.Screen));
    } else {
      fprintf(TheScreen.errout,\
              "UWM: Cannot alloc color: %d;%d;%d. Using WhitePixel.\n",
              xcol->red, xcol->green, xcol->blue);
      return(xcol->pixel = WhitePixel(disp,TheScreen.Screen));
    }
  }
}

void FreeColor(XColor *color)
{
  if((color->pixel != BlackPixel(disp, TheScreen.Screen))
     && (color->pixel != WhitePixel(disp, TheScreen.Screen)))
    XFreeColors(disp, TheScreen.colormap, &(color->pixel), 1, 0);
}

void ICCCM_WM_Init()
{
  char WM_Sx[64];
  XEvent event;
  Window otherwmswin;

  sprintf(WM_Sx,"WM_S%d",TheScreen.Screen);
  TheScreen.WM_Sx = XInternAtom(disp,WM_Sx,False);

  if(None != (otherwmswin = XGetSelectionOwner(disp, TheScreen.WM_Sx))) {
    printf("UWM: another icccm compliant window manager seems to be running\n");
    if(TheScreen.icccmFlags & (ICF_TRY_HARD | ICF_HOSTILE)) {
      int a, b;

      printf("UWM: --TryHard selected, overtaking control "); fflush(stdout);
      XSetSelectionOwner(disp, TheScreen.WM_Sx, TheScreen.inputwin,
                         TheScreen.start_tstamp);
      XSelectInput(disp, otherwmswin, StructureNotifyMask);
      for(a=0;a<10;a++){
        printf("."); fflush(stdout);
        if((b=XCheckWindowEvent(disp, otherwmswin, StructureNotifyMask, &event))
	   && (event.type == DestroyNotify)
	   && (event.xdestroywindow.window == otherwmswin)) break;
        if(a < 9) sleep(1);
      }
      printf("\n");
      if(b == False) {
        printf("UWM: Other WM still active,\n");
        if(TheScreen.icccmFlags & ICF_HOSTILE) {
          printf("     getting hostile.\n");
	  XKillClient(disp, otherwmswin);
        } else {
          SeeYa(1,"start uwm with --Hostile option to try harder to replace the wm\n\
    ATTENTION: replacing another running wm might terminate your X-session,\n\
               only use this option if you really know what you are doing!\n\
UWM ");
	}
      }
    } else {
     SeeYa(1,"start uwm with --TryHard option to try to replace the wm\n\
    ATTENTION: replacing another running wm might terminate your X-session,\n\
               only use this option if you really know what you are doing!\n\
UWM ");
    }
  }
  XSetSelectionOwner(disp, TheScreen.WM_Sx, TheScreen.inputwin,
                     TheScreen.start_tstamp);
  if(TheScreen.inputwin != XGetSelectionOwner(disp, TheScreen.WM_Sx))
    SeeYa(1,"Couldn't Acquire WM_Sx selection");
  event.type = ClientMessage;
  event.xclient.format = 32;
  event.xclient.window = TheScreen.root;
  event.xclient.message_type = XA_RESOURCE_MANAGER;
  event.xclient.data.l[0] = TheScreen.start_tstamp;
  event.xclient.data.l[1] = TheScreen.WM_Sx;
  event.xclient.data.l[2] = TheScreen.inputwin;
  event.xclient.data.l[3] = event.xclient.data.l[4] = 0;
  XSendEvent(disp, TheScreen.root, False, StructureNotifyMask, &event);
}

void InitUWM()
{
  char *env;
  XGCValues xgcv;
  XSetWindowAttributes xswa;
  XEvent event;

/*** some inits to let SeeYa know what to deallocate etc... ***/

  TheScreen.UltimateList= NULL;
  TheScreen.AppsMenu= NULL;
  TheScreen.UWMMenu= NULL;

/*** from this point on SeeYa can be called ***/

  TheScreen.Home = getenv("HOME");
  if(!TheScreen.Home) TheScreen.Home = ".";

  disp = XOpenDisplay(NULL);
  if(!disp)
    SeeYa(1, "Cannot open display");

  TheScreen.Screen = DefaultScreen (disp);
  TheScreen.width = DisplayWidth (disp,TheScreen.Screen);
  TheScreen.height = DisplayHeight (disp,TheScreen.Screen);
#ifndef DISABLE_BACKING_STORE
  TheScreen.DoesSaveUnders = DoesSaveUnders(ScreenOfDisplay(disp,
                                                            TheScreen.Screen));
  TheScreen.DoesBackingStore = DoesBackingStore(ScreenOfDisplay(disp,
                                                            TheScreen.Screen));
#else
  TheScreen.DoesSaveUnders = False;
  TheScreen.DoesBackingStore = False;
#endif

  /* Mark Display as close-on-exec (important for restarts and starting other wms) */
  if(fcntl (ConnectionNumber(disp), F_SETFD, 1) == -1)
    SeeYa(1,"Looks like something is wrong with the Display I got");

  TheScreen.root = RootWindow (disp, TheScreen.Screen);
  if(TheScreen.root == None)
    SeeYa(1, "No proper root Window available");

  XSetIOErrorHandler((XIOErrorHandler)ArmeageddonHandler);

  xswa.override_redirect = True;
  TheScreen.inputwin = XCreateWindow(disp, TheScreen.root, -2, -2, 1, 1, 0, 0,
                                     InputOnly, CopyFromParent,
                                     CWOverrideRedirect, &xswa);
  if(TheScreen.inputwin == None)
    SeeYa(1,"Couldn't initialize input and communication window");
  XMapWindow(disp, TheScreen.inputwin);

  XSelectInput(disp, TheScreen.inputwin, PropertyChangeMask);
  XStoreName(disp, TheScreen.inputwin, ""); /* obtain a time stamp */
  XWindowEvent(disp, TheScreen.inputwin, PropertyChangeMask, &event);
  TheScreen.now = TheScreen.start_tstamp = event.xproperty.time;
  XSelectInput(disp, TheScreen.inputwin, INPUTWIN_EVENTS);
    /* set the real event mask */

  ICCCM_WM_Init();

  XSetErrorHandler((XErrorHandler)RedirectErrorHandler);
  XSelectInput(disp, TheScreen.root, HANDLED_EVENTS);
  XSync(disp, False); /* in case we get thrown out: let RedirectErrorHandler */
	       /* process this before we switch to the default error handler */

  XSetErrorHandler((XErrorHandler)UWMErrorHandler);
    /* The real UWM-Error-handler */

  SetupCursors();  /* get the cursors we are going to use */
  InitHandlers();  /* set up the handler jump-tables */

  XDefineCursor(disp,TheScreen.root,TheScreen.Mice[C_DEFAULT]);
  TheScreen.colormap = DefaultColormap(disp,DefaultScreen(disp));

  /*** find out where ude is installed. ***/
  env = getenv("UDEdir");
  if(!env) {
    /* UDE_DIR is a macro that is defined in the Makefile (by automake) and
       it will contain the default ude directory which is the same as
       pkgdatadir. it will usually be /usr/local/share/ude */
    env = UDE_DIR;
  }
  if('/' != (*(strchr(env, '\0') - 1))) {
    TheScreen.udedir = MyCalloc(strlen(env) + 2, sizeof(char));
    sprintf(TheScreen.udedir, "%s/", env);
  } else {
    TheScreen.udedir = MyCalloc(strlen(env) + 1, sizeof(char));
    sprintf(TheScreen.udedir,"%s", env);
  }
  env = MyCalloc(strlen(TheScreen.udedir) + strlen("UDEdir=") + 1,
                 sizeof(char));
  sprintf(env, "UDEdir=%s", TheScreen.udedir);
  putenv(env);

  TheScreen.cppincpaths = malloc(sizeof(char) * (1 + strlen("-I ")
         + strlen(TheScreen.Home) + strlen("/.ude/config/ -I ") 
         + strlen(TheScreen.udedir) + strlen("config/ -I ./")));
  sprintf(TheScreen.cppincpaths, "-I %s/.ude/config/ -I %sconfig/ -I ./",
          TheScreen.Home, TheScreen.udedir);

  TheScreen.urdbcppopts = Initurdbcppopts();

/*** set up some uwm-specific stuff ***/

  UWMContext = XUniqueContext();   /* Create Context to store UWM-data */
  UWMGroupContext = XUniqueContext();   /* Create Context to store group data */
  TheScreen.MenuContext = XUniqueContext();   
  TheScreen.MenuFrameContext = XUniqueContext();   
    /* Create Contexts to store menu data */

  /* Atoms for selections */
  TheScreen.VERSION_ATOM = XInternAtom(disp,"VERSION",False);
  TheScreen.ATOM_PAIR = XInternAtom(disp,"ATOM_PAIR",False);
  TheScreen.TARGETS = XInternAtom(disp,"TARGETS",False);
  TheScreen.MULTIPLE = XInternAtom(disp,"MULTIPLE",False);
  TheScreen.TIMESTAMP = XInternAtom(disp,"TIMESTAMP",False);

  /* window management related Atoms */
  WM_STATE_PROPERTY = XInternAtom(disp,"WM_STATE",False);
  WM_CHANGE_STATE = XInternAtom(disp,"WM_CHANGE_STATE",False);
  WM_TAKE_FOCUS = XInternAtom(disp,"WM_TAKE_FOCUS",False);
  WM_DELETE_WINDOW = XInternAtom(disp,"WM_DELETE_WINDOW",False);
  WM_PROTOCOLS = XInternAtom(disp,"WM_PROTOCOLS",False);
  MOTIF_WM_HINTS = XInternAtom(disp,_XA_MOTIF_WM_HINTS,False);

  if(!(TheScreen.UltimateList = NodeListCreate()))
    SeeYa(1, "FATAL: out of memory!");

  /*** read configuration files ***/

  ReadConfigFile();
  PrepareIcons();

  /*** prepare menus ***/

  CreateUWMMenu();
  InitWSProcs();

  /*** set up more UWM-specific stuff ***/
  xgcv.function = GXinvert;
  xgcv.line_style = LineSolid;
  xgcv.line_width = settings.global_settings->BorderWidth;
  xgcv.cap_style = CapButt;
  xgcv.subwindow_mode = IncludeInferiors;
  TheScreen.rubbercontext = XCreateGC(disp,TheScreen.root,GCFunction|\
          GCCapStyle|GCLineStyle|GCLineWidth|GCSubwindowMode,&xgcv);
  xgcv.function = GXcopy;
  xgcv.line_style = LineSolid;
  xgcv.line_width = 0;
  xgcv.cap_style = CapButt;
  xgcv.foreground = BlackPixel(disp,TheScreen.Screen);
  TheScreen.blackcontext = XCreateGC(disp,TheScreen.root,GCFunction|\
            GCCapStyle|GCLineStyle|GCLineWidth|GCForeground,&xgcv);

  xgcv.function = GXcopy;
  xgcv.foreground = ActiveWSSettings->BackgroundLight->pixel;
  xgcv.line_width = 0;
  xgcv.line_style = LineSolid;
  xgcv.cap_style = CapButt;
  TheScreen.MenuLightGC = XCreateGC(disp, TheScreen.root, GCFunction
                                  | GCForeground | GCCapStyle | GCLineWidth
				  | GCLineStyle, &xgcv);
  xgcv.function = GXcopy;
  xgcv.foreground = ActiveWSSettings->BackgroundShadow->pixel;
  xgcv.line_width = 0;
  xgcv.line_style = LineSolid;
  xgcv.cap_style = CapButt;
  TheScreen.MenuShadowGC = XCreateGC(disp, TheScreen.root, GCFunction
                                  | GCForeground | GCCapStyle | GCLineWidth
				  | GCLineStyle, &xgcv);
  xgcv.function = GXcopy;
  xgcv.foreground = ActiveWSSettings->BackgroundColor->pixel;
  xgcv.line_width = 0;
  xgcv.line_style = LineSolid;
  xgcv.cap_style = CapButt;
  TheScreen.MenuBackGC = XCreateGC(disp, TheScreen.root, GCFunction
                                 | GCForeground | GCCapStyle | GCLineWidth
				 | GCLineStyle, &xgcv);
  xgcv.function = GXcopy;
  xgcv.foreground = ActiveWSSettings->ForegroundColor->pixel;
  xgcv.fill_style = FillSolid;
  xgcv.font = settings.global_settings->Font->xfs->fid;
  TheScreen.MenuTextGC = XCreateGC(disp,TheScreen.root, GCFunction
                                 | GCForeground | GCFillStyle | GCFont, &xgcv);

  XGrabKey(disp, XKeysymToKeycode(disp,XK_Right), UWM_MODIFIERS,
           TheScreen.root, True, GrabModeAsync, GrabModeAsync);
  XGrabKey(disp, XKeysymToKeycode(disp,XK_Left), UWM_MODIFIERS,
           TheScreen.root, True, GrabModeAsync, GrabModeAsync);
  XGrabKey(disp, XKeysymToKeycode(disp,XK_Up), UWM_MODIFIERS,
           TheScreen.root, True, GrabModeAsync, GrabModeAsync);
  XGrabKey(disp, XKeysymToKeycode(disp,XK_Down), UWM_MODIFIERS,
           TheScreen.root, True, GrabModeAsync, GrabModeAsync);
  XGrabKey(disp, XKeysymToKeycode(disp,XK_Page_Down), UWM_MODIFIERS,
           TheScreen.root, True, GrabModeAsync, GrabModeAsync);
  XGrabKey(disp, XKeysymToKeycode(disp,XK_Page_Up), UWM_MODIFIERS,
           TheScreen.root, True, GrabModeAsync, GrabModeAsync);
  XGrabKey(disp, XKeysymToKeycode(disp,XK_End), UWM_MODIFIERS,
           TheScreen.root, True, GrabModeAsync, GrabModeAsync);
  XGrabButton(disp, AnyButton, UWM_MODIFIERS, TheScreen.root,
              True, ButtonPressMask | ButtonReleaseMask, GrabModeAsync,
	      GrabModeAsync, None, None);

  BroadcastWorkSpacesInfo();
  UpdateDesktop();

  /* Set the background for the current desktop. */
  SetWSBackground();
}

uwm_global_settings global_settings;

uwm_settings settings = {
	  &global_settings,	/* global_settings */
	  0,			/* workspace_settings_count */
	  NULL			/* workspace_settings */
	};

int ReadConfigFile()
{
  int a;

/*** preinitialize global options ***/
  memset(&global_settings, 0, sizeof(global_settings));
  for(a = 0; a < UWM_GLOBAL_OPTION_NR; a ++) {
    if(uwm_global_index[a].default_val_string
       && ((uwm_global_index[a].type == UWM_S_INT)
           || (uwm_global_index[a].type == UWM_S_FLOAT))
       && (uwm_yy_to_setting_table[uwm_global_index[a].type][UWM_YY_STRING])) {
      YYSTYPE d;
      char *errmsg;
      d.string = MyStrdup(uwm_global_index[a].default_val_string);
      if(errmsg = uwm_yy_to_setting_table
                        [uwm_global_index[a].type][UWM_YY_STRING]
                        (&d, &(uwm_global_index[a]),
			 settings.global_settings)) {
        SeeYa(1, errmsg);
      }
    }
  }

  uwm_yyparse_wrapper("uwmrc.new");

#define derefptr(TYPE) ((TYPE *)(((void *)settings.global_settings) \
                                 + (uwm_global_index[a].offset)))
#define deref(TYPE) (*(derefptr(TYPE)))
  /* complete global options with defaults */
  for(a = 0; a < UWM_GLOBAL_OPTION_NR; a ++) {
    if(uwm_global_index[a].default_val_string
       && (uwm_global_index[a].type != UWM_S_INT)
       && (uwm_global_index[a].type != UWM_S_FLOAT)
       && (deref(void*) == NULL)
       && (uwm_yy_to_setting_table[uwm_global_index[a].type][UWM_YY_STRING])) {
      YYSTYPE d;
      char *errmsg;
      d.string = MyStrdup(uwm_global_index[a].default_val_string);
      if(errmsg = uwm_yy_to_setting_table
			[uwm_global_index[a].type][UWM_YY_STRING]
			(&d, &(uwm_global_index[a]),
			 settings.global_settings)) {
        SeeYa(1, errmsg);
      }
    }
  }
#ifdef OPTION_DEBUG
/***/printf("\n\n********************************************************\n");
/***/for(a = 0; a < UWM_GLOBAL_OPTION_NR; a ++) {
/***/  switch(uwm_global_index[a].type) {
/***/    case UWM_S_INT: printf("%s = %d\n", uwm_global_index[a].name,
/***/                           deref(int));
/***/         break;
/***/    case UWM_S_FLOAT: printf("%s = %f\n", uwm_global_index[a].name,
/***/                             deref(double));
/***/         break;
/***/    case UWM_S_STRING: printf("%s = %s\n", uwm_global_index[a].name,
/***/                             deref(char*));
/***/         break;
/***/    case UWM_S_FONT: if(deref(FontStruct*))
/***/                       printf("%s = %s\n", uwm_global_index[a].name,
/***/                              deref(FontStruct*)->name);
/***/         break;
/***/  }
/***/}
#endif /* OPTION_DEBUG */
#undef derefptr
#undef deref

  /* initialize gettext defaults */
  strncpy(uwm_default_ws_name, _("Default Workspace"),
          UWM_DEFAULT_NAME_LENGTH);
  uwm_default_ws_name[UWM_DEFAULT_NAME_LENGTH - 1] = '\0';

  /* we want at least one workspace */
  if((!settings.workspace_settings_count) || (!settings.workspace_settings)) {
    settings.workspace_settings_count = 1;
    settings.workspace_settings = MyCalloc(settings.workspace_settings_count,
					 sizeof(*settings.workspace_settings));
    settings.workspace_settings[0] = NULL;
  }

  /* complete workspace options with defaults */
  for(a = 0; a < settings.workspace_settings_count; a++) {
    int b;
    if(!settings.workspace_settings[a]) {
      settings.workspace_settings[a] = MyCalloc(1,
						sizeof(uwm_workspace_settings));
      memset(settings.workspace_settings[a], 0, sizeof(uwm_workspace_settings));
/* workspace options don't need preinitialisation because they currently */
/* don't contain simple datatypes and so memset is sufficient */
    }
#define derefptr(TYPE) ((TYPE *)(((void *)settings.workspace_settings[a]) \
                                 + (uwm_workspace_index[b].offset)))
#define deref(TYPE) (*(derefptr(TYPE)))
    for(b = 0; b < UWM_WORKSPACE_OPTION_NR; b ++) {
      if(uwm_workspace_index[b].default_val_string
         && (uwm_workspace_index[b].type != UWM_S_INT)
         && (uwm_workspace_index[b].type != UWM_S_FLOAT)
         && (deref(void*) == NULL)
         && (uwm_yy_to_setting_table[uwm_workspace_index[b].type]
	                            [UWM_YY_STRING])) {
        YYSTYPE d;
        char *errmsg;
        d.string = MyStrdup(uwm_workspace_index[b].default_val_string);
        if(errmsg = uwm_yy_to_setting_table
                          [uwm_workspace_index[b].type][UWM_YY_STRING]
                          (&d, &(uwm_workspace_index[b]),
			   settings.workspace_settings[a])) {
          SeeYa(1, errmsg);
        }
      }
    }
  }
  for(a = 0; a < settings.workspace_settings_count; a++) {
    int b;
#ifdef OPTION_DEBUG
/***/printf("\nws%d:\n", a);
/***/for(b = 0; b < UWM_WORKSPACE_OPTION_NR; b ++) {
/***/  switch(uwm_workspace_index[b].type) {
/***/    case UWM_S_INT: printf("  %s = %d\n", uwm_workspace_index[b].name,
/***/                           deref(int));
/***/         break;
/***/    case UWM_S_FLOAT: printf("  %s = %f\n", uwm_workspace_index[b].name,
/***/                             deref(double));
/***/         break;
/***/    case UWM_S_STRING: printf("  %s = %s\n", uwm_workspace_index[b].name,
/***/                             deref(char*));
/***/         break;
/***/    case UWM_S_FONT: if(deref(FontStruct*))
/***/                       printf("  %s = %s\n", uwm_workspace_index[b].name,
/***/                              deref(FontStruct*)->name);
/***/         break;
/***/    case UWM_S_PIXMAP: if(deref(uwm_image*))
/***/                       printf("  %s = 0x%X\n",
/***/                              uwm_workspace_index[b].name,
/***/                              deref(uwm_image*)->image);
/***/         break;
/***/    case UWM_S_COLOR: if(deref(XColor*))
/***/                       printf("  %s = #%.4X%.4X%.4X\n",
/***/                              uwm_workspace_index[b].name,
/***/                              deref(XColor*)->red,
/***/                              deref(XColor*)->green,
/***/                              deref(XColor*)->blue);
/***/         break;
/***/ }
/***/}
#endif /* OPTION_DEBUG */
  }
#undef derefptr
#undef deref
}

