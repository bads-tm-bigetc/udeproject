#ifndef UWM_H

#define UWM_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include <X11/cursorfont.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/xpm.h>

#include <config.h>

#include "nodes.h"
#include "menu.h"
#include "MwmUtil.h"

/*** some debugging and developement settings ***/

/*** DEVELOPERS PRIVILEGE: makes /dev/tty10 to be the standard
     error output... ***/
/*#define DEVEL  */

/*** will remove debugging code if not defined (defining this will
     make the program REALLY verbose...) ***/
/* #define DEBUG */

/* no changes necessary below this point */
#ifdef DEBUG
#define DBG(arg) arg
#else
#define DBG(arg)
#endif

/*** which icccm-version do we comply with ***/
#define ICCCM_MAJOR 2
#define ICCCM_MINOR 0


/*** some predefinitions... ***/

#define HANDLED_EVENTS (KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|EnterWindowMask|LeaveWindowMask|PropertyChangeMask|SubstructureNotifyMask|SubstructureRedirectMask)

#define INPUTWIN_EVENTS (KeyPressMask|StructureNotifyMask)

#define WINDOW_EVENTS (PropertyChangeMask) /*PropertyChangeMask*/

#define BORDER_EVENTS (ButtonPressMask|ButtonReleaseMask|ExposureMask)

#define TITLE_EVENTS (ExposureMask|ButtonPressMask|ButtonReleaseMask|EnterWindowMask)

#define FRAME_EVENTS (EnterWindowMask|LeaveWindowMask|SubstructureRedirectMask|SubstructureNotifyMask|FocusChangeMask)

#define UWM_MODIFIERS (ControlMask | Mod1Mask)

/*** used structures ***/ 

/* UDEScreen describes the managed screen */
#define CURSORS 12
#define C_DEFAULT 0
#define C_N 1
#define C_NO 2
#define C_O 3
#define C_SO 4
#define C_S 5
#define C_SW 6
#define C_W 7
#define C_NW 8
#define C_DRAG 9
#define C_BORDER 10
#define C_WINDOW 11

#define ICONWINS 7
#define I_ICONIFY 0
#define I_CLOSE 1
#define I_AUTORISE 2
#define I_BACK 3
#define I_KILL 4
#define I_MENU 5
#define I_REALLY 6

typedef struct {
  int Screen;
  Bool DoesSaveUnders, DoesBackingStore;
  Colormap colormap;
  unsigned int width, height;
  Window root, inputwin;

  Cursor Mice[CURSORS];
  GC rubbercontext, blackcontext;
  GC MenuTextGC, MenuLightGC, MenuShadowGC, MenuBackGC;

  NodeList *UltimateList;        /* contains all ultimized windows,
                                    data of type UltimateContext * */
  XContext MenuContext, MenuFrameContext;

  char *udedir;
  char *cppcall, *cppincpaths, *urdbcppopts;
  char *Home;

  Atom WM_Sx, VERSION_ATOM, ATOM_PAIR, TARGETS, MULTIPLE, TIMESTAMP;
  Time start_tstamp;
  Time now;

  unsigned int ActiveWorkspace;

  FILE *errout;

  struct {
    Window IconWins[ICONWINS];
    Pixmap IconPixs[ICONWINS];
    Pixmap IconSelectPixs[ICONWINS];
    Pixmap shape;
    Window IconParent;
  } icons;

  Menu *AppsMenu;
  Menu *UWMMenu;
} UDEScreen;

extern UDEScreen TheScreen;

#define StampTime(TIME) (TheScreen.now = ((TheScreen.now < TIME) || (TheScreen.now > (TIME + (1<<(sizeof(TheScreen.now)*8-1))))) ? TIME : TheScreen.now)
#define TimeStamp TheScreen.now

#define ButtonCount(CNT) ((((CNT) & Button1Mask) ? 1 : 0) \
                          + (((CNT) & Button2Mask) ? 1 : 0) \
                          + (((CNT) & Button3Mask) ? 1 : 0) \
                          + (((CNT) & Button4Mask) ? 1 : 0) \
                          + (((CNT) & Button5Mask) ? 1 : 0))

/*** Window-Context structure ***/
typedef struct _WinGroup *WinGroupPtr;

/* flags */
#define RISEN         (1<<0)    /* window autorisen? */
#define PLACEIT       (1<<1)    /* Autoplace window before mapping? */
#define SHAPED        (1<<2)    /* window shaped? */
#define ACTIVE_BORDER (1<<3)    /* window border drawn as active? */

/* wm-protocol-flags */
#define TAKE_FOCUS    (1<<0)
#define DELETE_WINDOW (1<<1)

typedef struct _UltimateContext
{
  Window win,frame; /* frame: if created: Frame-Window else: None */
  Window border;    /* a child of the frame to set the Mouse pointer correct */

  int BorderWidth,OldBorderWidth;
  XWindowAttributes Attributes,Attr;  /* Attributes: window; Attr: Frame */

  struct {
    int x, y, width, height;
    char *name, *iconname;
    Window win;  /* a child of the frame containing the window's title string */
  } title;

  struct {
    int x,y,w,h;               /* original window coordinates before autorise */
    int minw,minh,maxw,maxh;          /* How big can I get? */
    int wi,hi;                        /* increases */
    int bw,bh;                        /* offsets considering the increases... */
    CARD32 gravity;                 /* wm gravity */
  } ra;                           /*** ResizeAttributes ***/

  XWMHints *WMHints;              /* some information... */
  PropMotifWmHints *MotifWMHints; /* even more information... */
  Window TransientFor;            /* who are we transient for or None */
  WinGroupPtr group;                /* group of window or NULL */
  unsigned char ProtocolFlags;    /* WM_PROTOCOLS */

  unsigned long uwmstate;         /* uwm's idea of the window's state */
  unsigned long wmstate;          /* the state noted in WM_STATE (icccm 2.0) */
  unsigned char flags;
  unsigned short expected_unmap_events;
  unsigned short own_unmap_events;
  short WorkSpace;
} UltimateContext;

#define IsIconic(UC) ((uc)->uwmstate == IconicState)
#define IsNormal(UC) ((uc)->uwmstate == NormalState)
#define IsWithdrawn(UC) ((uc)->uwmstate == WithdrawnState)

#define SeemsIconic(UC) ((uc)->wmstate == IconicState)
#define SeemsNormal(UC) ((uc)->wmstate == NormalState)
#define SeemsWithdrawn(UC) ((uc)->wmstate == WithdrawnState)

/*** Structure describing an application programm that can be called by uwm ***/
typedef struct {
  char *command;
} AppStruct;

typedef void (*HandlerTable)();

/*** prototypes ***/

void CleanUp(Bool StopScript);
                    /* Procedure to clean up, only needed before Restart */
void SeeYa(int ecode, char *vocalz);   /* Procedure to say good bye */
void ShellQuit(int dummy);

#endif
