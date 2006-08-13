#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <X11/Xlib.h>

#include <stdio.h>
#include "ude-types.h"
#include "ude-desktop.h"
#include "ude-settings.h"

#include "ude-raster.xbm"

Atom UDEContext;

/**************************** PRIVATE ************************************/

/* X11 error handler for debugging purpose */
void
ude_error_handler (Display *disp, XErrorEvent *Err)
{
  char et[256];
  
  XGetErrorText (disp, Err->error_code, et, 255);
  fprintf (stderr, "UDElib: An error #%d occured:\n%s\n", Err->error_code, et);
  fprintf (stderr, "        It was caused by command (Major-Minor): %d-%d\n",
	   Err->request_code,Err->minor_code);
  fprintf (stderr, "        Look up the meanings in X11/Xproto.h\n");
}

/**************************** END OF PRIVATE ******************************/

/*** not really necessary yet. however please use it to stay compatible. ***/
void ude_clean_up(UDEDesktop *desk)
{
  if(desk->internal.raster_pixmap != None)
    XFreePixmap(desk->disp, desk->internal.raster_pixmap);
  XCloseDisplay(desk->disp);
}


/* will initialize the library:
   open a display, initialize the rest of the UDEDesktop structure and return
   a status value as well as the desktop structure.
   return values: UDE_SUCCESS  - success.
                  UDE_FAIL     - fatal error: couldn't initialize libUDE
                  UDE_BADVAL   - success, but couldn't find uwm or uds running,
                                 using default values. */
char ude_initialize (UDEDesktop *desktop)
{
  XSetErrorHandler ((XErrorHandler)ude_error_handler);
  desktop->disp= XOpenDisplay (NULL);
  if(!desktop->disp)
    return UDE_FAIL;
  XSelectInput(desktop->disp,
               RootWindow(desktop->disp,DefaultScreen(desktop->disp)),
               PropertyChangeMask);
  UDEContext= XInternAtom (desktop->disp, "_UDE_UDEContext", False);

  ude_init_settings(desktop->disp);

#ifdef HAVE_XSHAPEQUERYEXTENSION
  XShapeQueryExtension(desktop->disp, &desktop->shape_ext.event_base,
                       &desktop->shape_ext.error_base);
#else
  desktop->shape_ext.event_base = -1;
  desktop->shape_ext.error_base = -1;
#endif

  desktop->ActualColors = NULL;
  desktop->ActiveWorkSpace = -1;
  desktop->WorkSpaces = -1;
  desktop->internal.sgrabstat = 0;
  desktop->internal.pgrabstat = NULL;
  ude_update_UDEDesktop(desktop);

  desktop->internal.raster_pixmap = XCreateBitmapFromData( desktop->disp, 
               RootWindow(desktop->disp,DefaultScreen(desktop->disp)),
               _UDE_raster_bits, _UDE_raster_width, _UDE_raster_height );
  if(None == desktop->internal.raster_pixmap) {
    ude_clean_up(desktop->disp);
    return UDE_FAIL;
  }

  return UDE_SUCCESS;
}

void
ude_error_exit(int ecode, char *message)
{
  fprintf(stderr,"UDE: %s",message);
  exit(ecode);
}
