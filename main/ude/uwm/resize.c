/***  RESIZE.C: Contains the handling routines for signals and events
                                invoked during a resize process  ***/

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

#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "uwm.h"
#include "handlers.h"
#include "windows.h"
#include "rubber.h"
#include "init.h"
#include "special.h"

extern UDEScreen TheScreen;
extern Display *disp;
extern UltimateContext *ActiveWin;
extern InitStruct InitS;

int ox1,ox2,oy1,oy2,x1,x2,y1,y2,minw,maxw,minh,maxh,wi,hi;
int bw,bh;

short risen;  /*  0: normal resize; 1: fullscreen; 2:old size */
short risenstart; /* started in risen mode? */

#define LACTIVE (1<<0)
#define RACTIVE (1<<1)
#define TACTIVE (1<<2)
#define BACTIVE (1<<3)
#define XACTIVE (1<<4)
#define YACTIVE (1<<5)
unsigned char borderstat,oldstat;

void SqueezeIt(int mousex,int mousey)
{
  if(!(borderstat & TACTIVE)) 
    if(mousey <= (oy1+ActiveWin->BorderWidth+TheScreen.TitleHeight)){
      borderstat&=(~(BACTIVE|YACTIVE));
      borderstat|=TACTIVE;
      y2=oy2;
    }
  if(!(borderstat & BACTIVE)) 
    if(mousey >= (oy2-ActiveWin->BorderWidth)){
      borderstat&=(~(TACTIVE|YACTIVE));
      borderstat|=BACTIVE;
      y1=oy1;
    }
  if(!(borderstat & LACTIVE)) 
    if(mousex <= (ox1+ActiveWin->BorderWidth)){
      borderstat&=(~(RACTIVE|XACTIVE));
      borderstat|=LACTIVE;
      x2=ox2;
    }
  if(!(borderstat & RACTIVE)) 
    if(mousex >= (ox2-ActiveWin->BorderWidth)){
      borderstat&=(~(LACTIVE|XACTIVE));
      borderstat|=RACTIVE;
      x1=ox1;
    }

  if(borderstat & TACTIVE){
    if((!(borderstat & YACTIVE)) && ((mousey<oy1)||\
      (mousey>oy1+ActiveWin->BorderWidth+TheScreen.TitleHeight)))
      borderstat|=YACTIVE;
    if(borderstat & YACTIVE){
      y1=mousey;
      if((y2-y1)<minh) y1=y2-minh;
      if((y2-y1)>maxh) y1=y2-maxh;
      y1=y2-((int)((y2-y1-bh)/hi))*hi-bh;
    }
  }
  if(borderstat & BACTIVE){
    if((!(borderstat & YACTIVE)) && ((mousey>oy2)||\
      (mousey<oy2-ActiveWin->BorderWidth)))
      borderstat|=YACTIVE;
    if(borderstat & YACTIVE){
      y2=mousey;
      if((y2-y1)<minh) y2=y1+minh;
      if((y2-y1)>maxh) y2=y1+maxh;
      y2=y1+((int)((y2-y1-bh)/hi))*hi+bh;
    }
  }
  if(borderstat & LACTIVE){
    if((!(borderstat & XACTIVE)) && ((mousex<ox1)||\
      (mousex>ox1+ActiveWin->BorderWidth)))
      borderstat|=XACTIVE;
    if(borderstat & XACTIVE){
      x1=mousex;
      if((x2-x1)<minw) x1=x2-minw;
      if((x2-x1)>maxw) x1=x2-maxw;
      x1=x2-((int)((x2-x1-bw)/wi))*wi-bw;
    }
  }
  if(borderstat & RACTIVE){
    if((!(borderstat & XACTIVE)) && ((mousex>ox2)||\
      (mousex<ox2-ActiveWin->BorderWidth)))
      borderstat|=XACTIVE;
    if(borderstat & XACTIVE){
      x2=mousex;
      if((x2-x1)<minw) x2=x1+minw;
      if((x2-x1)>maxw) x2=x1+maxw;
      x2=x1+((int)((x2-x1-bw)/wi))*wi+bw;
    }
  }
  if(borderstat!=oldstat){
    oldstat = borderstat;
    switch(borderstat&(RACTIVE|LACTIVE|TACTIVE|BACTIVE)){
      case RACTIVE: 
        XChangeActivePointerGrab(disp,PointerMotionMask|ButtonPressMask|\
          ButtonReleaseMask,TheScreen.Mice[C_O],TimeStamp);
        break;
      case RACTIVE|TACTIVE:
        XChangeActivePointerGrab(disp,PointerMotionMask|ButtonPressMask|\
          ButtonReleaseMask,TheScreen.Mice[C_NO],TimeStamp);
        break;
      case TACTIVE:
        XChangeActivePointerGrab(disp,PointerMotionMask|ButtonPressMask|\
          ButtonReleaseMask,TheScreen.Mice[C_N],TimeStamp);
        break;
      case TACTIVE|LACTIVE:
        XChangeActivePointerGrab(disp,PointerMotionMask|ButtonPressMask|\
          ButtonReleaseMask,TheScreen.Mice[C_NW],TimeStamp);
        break;
      case LACTIVE:
        XChangeActivePointerGrab(disp,PointerMotionMask|ButtonPressMask|\
          ButtonReleaseMask,TheScreen.Mice[C_W],TimeStamp);
        break;
      case LACTIVE|BACTIVE:
        XChangeActivePointerGrab(disp,PointerMotionMask|ButtonPressMask|\
          ButtonReleaseMask,TheScreen.Mice[C_SW],TimeStamp);
        break;
      case BACTIVE:
        XChangeActivePointerGrab(disp,PointerMotionMask|ButtonPressMask|\
          ButtonReleaseMask,TheScreen.Mice[C_S],TimeStamp);
        break;
      case BACTIVE|RACTIVE:
        XChangeActivePointerGrab(disp,PointerMotionMask|ButtonPressMask|\
          ButtonReleaseMask,TheScreen.Mice[C_SO],TimeStamp);
        break;
      default:
        XChangeActivePointerGrab(disp,PointerMotionMask|ButtonPressMask|\
          ButtonReleaseMask,TheScreen.Mice[C_DEFAULT],TimeStamp);
        break;
    }
  }
  SqueezeRubber(x1,y1,x2-x1,y2-y1);
}

void RiseIt()
{
  if((!risenstart)&&(risen!=1)){
    x1=y1=0;
    x2=(maxw>TheScreen.width)?(bw+((int)((TheScreen.width-bw-1)/wi))*wi):maxw;
    y2=(maxh>TheScreen.height)?(bh+((int)((TheScreen.height-bh-1)/hi))*hi):maxh;
    SqueezeRubber(x1,y1,x2-x1,y2-y1);
    risen=1;
  } else {
    x1=ActiveWin->ra.x;y1=ActiveWin->ra.y;
    x2=x1+ActiveWin->ra.w;y2=y1+ActiveWin->ra.h;
    SqueezeRubber(x1,y1,x2-x1,y2-y1);
    risen=2;
    risenstart=False;
  }
    XChangeActivePointerGrab(disp,PointerMotionMask|ButtonPressMask|\
            ButtonReleaseMask,TheScreen.Mice[C_DEFAULT],TimeStamp);
}

void UnriseIt(int x,int y)
{
  x1=ox1;x2=ox2;y1=oy1;y2=oy2;
  borderstat=0;
  SqueezeIt(x,y);
  risen=0;
}

/* StartResizing installs the handlers used to resize a window
 * requires absolute coordinates (relative to root)           */

void StartResizing(UltimateContext *uc,int x,int y)
{
  risen=0;

  minh=uc->ra.minh;    /*** lazy dayze ***/
  maxh=uc->ra.maxh;
  minw=uc->ra.minw;
  maxw=uc->ra.maxw;
  wi=uc->ra.wi;
  hi=uc->ra.hi;
  bw=uc->ra.bw;
  bh=uc->ra.bh;

  x1=ox1=uc->Attr.x;
  x2=ox2=((uc->Attr.x)+(uc->Attr.width));
  y1=oy1=uc->Attr.y;
  y2=oy2=((uc->Attr.y)+(uc->Attr.height));
  if(!(uc->flags & RISEN)){
    ActiveWin->ra.x=ox1;
    ActiveWin->ra.y=oy1;
    ActiveWin->ra.w=ox2-ox1;
    ActiveWin->ra.h=oy2-oy1;
    risenstart=False;
  } else risenstart=True;
  borderstat=oldstat=0;

  GrabPointer(TheScreen.root,PointerMotionMask|ButtonPressMask|\
                                        ButtonReleaseMask,None);
  StartRubber(x1,y1,x2-x1,y2-y1,ActiveWin->BorderWidth);
  GrabServer();
  InstallResizeHandle();
  if(!(uc->flags & RISEN)) SqueezeIt(x,y);

  DBG( fprintf(TheScreen.errout,"min w: %u; h: %u max w: %u; h: %u; wi: %u; hi: %u\n",\
       minw,minh,maxw,maxh,wi,hi);)
}

void ResizeButtons(int a,XEvent *event)
{
  switch(InitS.ResizeButtons[a]){
    case 'U': UnriseIt(event->xbutton.x_root,event->xbutton.y_root);break;
    case 'A': RiseIt();break;
    case 'R': break;
  }
}

void ResizeButtonPress(XEvent *event)
{
  DBG(fprintf(TheScreen.errout,"ResizeButtonPress\n");)
  StampTime(event->xbutton.time);
  switch(event->xbutton.button){
    case Button1: ResizeButtons(0,event);break;
    case Button2: ResizeButtons(1,event);break;
    case Button3: ResizeButtons(2,event);break;
    case Button4:
    case Button5: break;
  }
}

void ResizeMotion(XEvent *event)
{
  StampTime(event->xmotion.time);
  if(!risen) SqueezeIt(event->xmotion.x_root,event->xmotion.y_root);
}

void ResizeButtonRelease(XEvent *event)
{
  int x,y,width,height;
  DBG(fprintf(TheScreen.errout,"ResizeButtonRelease\n");)
  StampTime(event->xbutton.time);
  if(ButtonCount(event->xbutton.state)>1) return;
  if(risen==1) ActiveWin->flags |= RISEN;
  else ActiveWin->flags &=~ RISEN;
  StopRubber(&x,&y,&width,&height);
  MoveResizeWin(ActiveWin,x,y,width,height);
  UngrabPointer();
  ReinstallDefaultHandle();
  UngrabServer();
}
