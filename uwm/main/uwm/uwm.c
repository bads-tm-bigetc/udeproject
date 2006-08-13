/***  UWM.C:  Main file of UWM  ***/

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#else
#include <string.h>
#endif

#include "uwm.h"
#include "init.h"
#include "windows.h"
#include "handlers.h"
#include "menu.h"
#include "special.h"
#include "properties.h"

Display *disp=NULL; 	/* 'global' display to Server... */ 
UDEScreen TheScreen;    /* structure that keeps all important global info */
XContext UWMContext;    /* WM-Concidering information connected to a window */
XContext UWMGroupContext;    /* information connected to a window group */
Atom WM_STATE_PROPERTY;
Atom WM_CHANGE_STATE;
Atom WM_TAKE_FOCUS;
Atom WM_DELETE_WINDOW;
Atom WM_PROTOCOLS;
Atom MOTIF_WM_HINTS;
InitStruct InitS;

extern HandlerTable *Handle;
extern int ShapeEvent;

/*** Exit-Procedures ***/

extern pid_t ScreenCommandPID;
void CleanUp(Bool StopScript)
{
  Node *n;
  struct stat stats;
  char rmstr[256],*p;

/*  if(ScreenCommandPID>0) if(!kill(ScreenCommandPID,SIGTERM)) sleep(1); */
                     /* terminate running ScreenCommand and not leave a zombie*/
  if(TheScreen.UltimateList) if(n=TheScreen.UltimateList->first)
    while(n) {
      XMapWindow(disp, ((UltimateContext *)(n->data))->win);
      SetSeemsMapState(n->data, ((UltimateContext *)(n->data))->uwmstate);
      n = PlainDeUltimizeWin(n->data, True);
    }
  if(TheScreen.AppsMenu)     DestroyMenu(TheScreen.AppsMenu);
  if(TheScreen.UWMMenu)      DestroyMenu(TheScreen.UWMMenu);

  if(disp)                   XCloseDisplay(disp);
  signal(SIGCHLD,SIG_DFL);

  if((InitS.StopScript[0]!='\n')&&(InitS.StopScript[0]!='\0')&&StopScript) {
    sprintf(rmstr,"%s/.ude/config/%s",TheScreen.Home,InitS.StopScript);
    if(p=strchr(rmstr,' ')) (*p)='\0';
    if(stat(rmstr,&stats))
      if((errno==ENOENT)||(errno==EACCES)) {
        sprintf(rmstr,"%sconfig/%s",TheScreen.udedir,InitS.StopScript);
        if(p=strchr(rmstr,' ')) (*p)='\0';
        if(stat(rmstr,&stats))
          if((errno==ENOENT)||(errno==EACCES)) {
            sprintf(rmstr,"%s",InitS.StopScript);
            p=NULL;
          }
      }
    if(p) *p=' ';
    system(rmstr);                 /* execute Stop-Script and wait for it */
  }
}

void SeeYa(int ecode, char *vocalz)
{
  CleanUp(True);

  if(vocalz) fprintf(TheScreen.errout,"\nUWM: %s - terminating (sorry)\n\n",vocalz);
  else if(ecode)
    fprintf(TheScreen.errout,"\nUWM: Exiting, I guess there is some reason for it\n\n");
  else fprintf(TheScreen.errout,"\nUWM: Exiting due to user request. (I guess...)\n\n");
  exit(ecode);
}

void CatchWindows()
{
  Window dummy,*children;
  unsigned int number,a,b;
  UltimateContext *uc;
  XWMHints *hints;

  XQueryTree(disp,TheScreen.root,&dummy,&dummy,&children,&number);
  for(a=0;a<number;a++) if(children[a]) {
    if(hints=XGetWMHints(disp,children[a])) {
      if(hints->flags & IconWindowHint) {
        for(b=0;b<number;b++) 
          if(children[b]==hints->icon_window) {
            children[b]=None;
            break;
          }
      }
      XFree((char *) hints);
    }
  }

  for(a=0;a<number;a++){
    if(children[a]) {
      int format;
      unsigned long number,bytesafter;
      Atom type;
      CARD32 *data;
      if(Success!=XGetWindowProperty(disp, children[a], WM_STATE_PROPERTY, 0, 2,
                                     False, WM_STATE_PROPERTY, &type,
				     &format, &number, &bytesafter,
				     (unsigned char **)&data)) data = NULL;
      if(uc = UltimizeWin(children[a])) {
        if(!uc->Attributes.override_redirect) {
	  CARD32 state = WithdrawnState;
	
          if((uc->WMHints)&&(uc->WMHints->flags & StateHint)&&
             (uc->WMHints->initial_state==IconicState)){
	    state = IconicState;
          } else if((uc->WMHints)&&(uc->WMHints->flags & StateHint)&&
                    (uc->WMHints->initial_state==WithdrawnState)){
	    state = WithdrawnState;
          } else if(uc->Attributes.map_state!=IsUnmapped) {
	    state = NormalState;
          }
          if(data) {
	    if((number == 2) && (type == WM_STATE_PROPERTY)
	       && (format == 32))  state = data[0];
	    XFree(data);
          }
	  switch(state) {
	    case IconicState:
               IconifyWin(uc);
	       break;
	    case WithdrawnState:
	       SetIsMapState(uc, WithdrawnState);
	       break;
	    case NormalState:
               DisplayWin(uc);
	       break;
	  }
        }
      }
    }
  }
  if(number>0) XFree((char *)children);
}

void ManagEm()
{
  while(1)
  {
    static XEvent event;

    XNextEvent(disp,&event);
    if(event.type<LASTEvent){  /* Standard X11-events */
      if(Handle[event.type]) (*Handle[event.type])(&event);
    } else {                   /* Extensions */
      if(event.type==ShapeEvent) HandleShape(&event);
    }
  }
}

void ShellQuit(int dummy)
{
  wait(NULL);                /*** We don't want to leave a zombie, eh?! ***/
  signal(SIGCHLD,ShellQuit); /* ugly non-std-signal-processing-patch (arrgh!) */
}

void TermSig(int dummy)
{
  SeeYa(0,"Signal received!");
}

const char uwm_versionstring[] =
  "\n     UWM - the UDE Window Manager\nVersion " UWMVERSION "\n\n";

const char uwm_helpstring[] =
  "Options & Switches:\n"
  "  --NoStartScript     prevents uwm from executing StartScript\n"
  "  --NoStopScript      prevents uwm from executing StopScript\n"
  "  --TryHard           try to replace another running icccm compliant wm\n"
  "  --Hostile           try harder replacing another icccm \'compliant\' wm\n"
  "  --StayAlive         don\'t give up windowmanagement control voluntarily\n"
  "  --version, -v       print version information and exit\n\n";

void ParseCommandLine(int argc,char **argv)
{
  int a, b;

  InitS.StartScript[0] = '\0';
  InitS.StopScript[0] = '\0';
  TheScreen.icccmFlags = 0;

  for(a = 1; a < argc; a++) {
    if(!strcmp("--help",argv[a])) {
      printf("%s%s", uwm_versionstring, uwm_helpstring);
      exit(0);
    } else if(!strcmp("--NoStartScript", argv[a])) {
      InitS.StartScript[0]='\n';
    } else if(!strcmp("--NoStopScript", argv[a])) {
      InitS.StopScript[0]='\n';
    } else if(!strcmp("--TryHard", argv[a])) {
      TheScreen.icccmFlags |= ICF_TRY_HARD;
    } else if(!strcmp("--Hostile", argv[a])) {
      TheScreen.icccmFlags |= ICF_HOSTILE;
    } else if(!strcmp("--StayAlive", argv[a])) {
      TheScreen.icccmFlags |= ICF_STAY_ALIVE;
    } else if((!strcmp("--version", argv[a])) || (!strcmp("-v", argv[a]))) {
      printf("%s", uwm_versionstring);
      exit(0);
    } else {
      printf("UWM: unknown option %s\nType %s --help for more info\n",
	     argv[a], argv[0]);
      exit(0);
    }
  }
}

/*** MAIN ***/

int main(int argc,char **argv)
{
  struct stat stats;
  char *dispstr, *hdispstr, *xds;
  char *rmstr, *p;

#ifdef ENABLE_NLS
  /* init i18n */
  /*setlocale (LC_ALL, "");  don't need everything, LC_NUMERIC will confuse
                             initialisation (decimal points)*/
#ifdef LC_COLLATE
  setlocale(LC_COLLATE, "");
#endif
#ifdef LC_CTYPE
  setlocale(LC_CTYPE, "");
#endif
#ifdef LC_MONETARY
  setlocale(LC_MONETARY, "");
#endif
#ifdef LC_TIME
  setlocale(LC_TIME, "");
#endif
#ifdef LC_MESSAGES
  setlocale(LC_MESSAGES, "");
#endif
#ifdef LC_RESPONSES
  setlocale(LC_RESPONSES, "");
#endif
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
#endif /* ENABLE_NLS */

#ifdef DEVEL
  TheScreen.errout = fopen("/dev/tty10","w");
  fprintf(TheScreen.errout,"UWM: Using this display for Error output!\n");
  fflush(TheScreen.errout);
#else
  TheScreen.errout = stderr;
#endif

  ParseCommandLine(argc, argv);

  CheckCPP(); /* do this before initializing SIGCHLD */

/*** from this point on MyOpen can be called ***/

  signal(SIGCHLD, ShellQuit); /* collect zombies */

  signal(SIGTERM, TermSig);   /* exit gracefully on signal */
  signal(SIGHUP,  TermSig);

  InitUWM();

#ifdef HAVE_PUTENV
  xds = XDisplayString(disp);
  dispstr = MyCalloc(strlen(xds) + 9, sizeof(char));
  sprintf(dispstr, "DISPLAY=%s", xds);
  putenv(dispstr);
#ifdef HAVE_UNAME
  if(strchr(xds,':') == xds) {
    struct utsname hname;
    uname(&hname);
    hdispstr = MyCalloc(strlen(xds) + strlen(hname.nodename) + 13,
			sizeof(char));
    sprintf(hdispstr, "HOSTDISPLAY=%s%s", hname.nodename, xds);
  } else {
    hdispstr = MyCalloc(strlen(xds) + 13, sizeof(char));
    sprintf(hdispstr, "HOSTDISPLAY=%s", xds);
  }
  putenv(hdispstr);
#endif /* HAVE_UNAME */
#endif /* HAVE_PUTENV */

  CatchWindows();

  if((InitS.StartScript[0]!='\n')&&(InitS.StartScript[0]!='\0')) {
    sprintf(rmstr,"%s/.ude/config/%s",TheScreen.Home,InitS.StartScript);
    if(p=strchr(rmstr,' ')) (*p)='\0';
    if(stat(rmstr,&stats))
      if((errno==ENOENT)||(errno==EACCES)) {
        sprintf(rmstr,"%sconfig/%s",TheScreen.udedir,InitS.StartScript);
        if(p=strchr(rmstr,' ')) (*p)='\0';
        if(stat(rmstr,&stats))
          if((errno==ENOENT)||(errno==EACCES)) {
            sprintf(rmstr,"%s",InitS.StartScript);
            p=NULL;
          }
      }
    if(p) *p=' ';
    MySystem(rmstr);                 /* execute Startup-Script  */
  }

  ManagEm();                     /* what a window manager is supposed to do */
  SeeYa(0,NULL);
}
