/***  WIDGETS.C: Contains routines for the UWM-widget-drawing  ***/

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

#include <X11/Xlib.h>

#include "uwm.h"
#include "init.h"

extern Display *disp;
extern UDEScreen TheScreen;
extern InitStruct InitS;


void DrawBevel(Drawable win,int x1,int y1,int x2,int y2,int width,GC NW,GC SE)
{
  int a;
  for(a=0;a<width;a++){
    XDrawLine(disp,win,NW,x1+a,y1+a,x2-a-1,y1+a);
    XDrawLine(disp,win,NW,x1+a,y1+a+1,x1+a,y2-a);
    XDrawLine(disp,win,SE,x1+a+1,y2-a,x2-1-a,y2-a);
    XDrawLine(disp,win,SE,x2-a,y1+a,x2-a,y2-a);
  }
}


/* Draw bevels onto a window. I use this function rather
   than the widgets.c DrawBevel function because I want to draw a
   shadowed border too, and this way I can have other effects.
   Partially by Adam Sampson. */
void DrawFrameBevel(UltimateContext *uc,char Active)
{
  XGCValues xgcv;
  int xa, ya, xb, yb, xc, yc, xd, yd, i;
  GC LightGC, ShadowGC;

  if(!TheScreen.FrameBevelWidth) return;

  xgcv.function=GXcopy;
  xgcv.foreground=Active
                  ? TheScreen.ActiveLight[TheScreen.desktop.ActiveWorkSpace]\
                  :TheScreen.InactiveLight[TheScreen.desktop.ActiveWorkSpace];
  xgcv.line_width=0;
  xgcv.line_style=LineSolid;
  xgcv.cap_style=CapButt;
  LightGC=XCreateGC(disp,uc->frame,GCFunction|GCForeground|\
                  GCCapStyle|GCLineWidth|GCLineStyle,&xgcv);

  xgcv.function=GXcopy;
  xgcv.foreground=Active
                  ? TheScreen.ActiveShadow[TheScreen.desktop.ActiveWorkSpace]\
                  : TheScreen.InactiveShadow[TheScreen.desktop.ActiveWorkSpace];
  xgcv.line_width=0;
  xgcv.line_style=LineSolid;
  xgcv.cap_style=CapButt;
  ShadowGC=XCreateGC(disp,uc->frame,GCFunction|GCForeground|\
                   GCCapStyle|GCLineWidth|GCLineStyle,&xgcv);

  if(!(uc->flags & SHAPED)){

    xc = yc = 0;
    xd = uc->Attr.width - 1;
    yd = uc->Attr.height - 1;
  
    for(i=0; i<TheScreen.FrameBevelWidth; i++) {
      xa = xc + i; xb = xd - i;
      ya = yc + i; yb = yd - i;
      XDrawLine(disp,uc->border, i?LightGC:ShadowGC, xa, ya, xb, ya);
      XDrawLine(disp,uc->border, i?ShadowGC:TheScreen.blackcontext,xb,ya,xb,yb);
      XDrawLine(disp,uc->border, i?ShadowGC:TheScreen.blackcontext,xb,yb,xa,yb);
      XDrawLine(disp,uc->border, i?LightGC:ShadowGC, xa, yb, xa, ya);
    }

    if(((i=(uc->BorderWidth-TheScreen.FrameBevelWidth-1)) > 3) &&\
                            (InitS.BorderTitleFlags & BT_GROOVE)){
      i=i/2+TheScreen.FrameBevelWidth;
      if(InitS.BorderTitleFlags & BT_CENTER_TITLE) {
        DrawBevel(uc->border,i-1,2*uc->BorderWidth,i,uc->Attr.height-\
                                2*uc->BorderWidth,1,ShadowGC,LightGC);
        DrawBevel(uc->border,2*uc->BorderWidth,i-1,uc->Attr.width-\
                           2*uc->BorderWidth,i,1,ShadowGC,LightGC);
        DrawBevel(uc->border,uc->Attr.width-i-1,2*uc->BorderWidth,\
               uc->Attr.width-i,uc->Attr.height-2*uc->BorderWidth,\
                                               1,ShadowGC,LightGC);
        DrawBevel(uc->border,2*uc->BorderWidth,uc->Attr.height-i-1,\
                 uc->Attr.width-2*uc->BorderWidth,uc->Attr.height-i\
                                               ,1,ShadowGC,LightGC);
      } else {
        DrawBevel(uc->border,i-1,i-1,uc->Attr.width-i,uc->Attr.height-i,1,\
                                                         ShadowGC,LightGC);
        DrawBevel(uc->border,i,i,uc->Attr.width-i-1,uc->Attr.height-i-1,1,\
                                                         LightGC,ShadowGC);
      }
    }
    if(InitS.BorderTitleFlags&BT_LINE)
      DrawBevel(uc->border,uc->BorderWidth-1,uc->BorderWidth+TheScreen.\
                          TitleHeight-1,uc->Attr.width-uc->BorderWidth,\
                                     uc->Attr.height-uc->BorderWidth,1,\
                         TheScreen.blackcontext,TheScreen.blackcontext);
  } else {
    XShapeQueryExtents(disp,uc->win,&i,&xa,&xa,&xb,&yb,&xa,&xa,&xa,&xa,&xa);
    DrawBevel(uc->border,0,0,uc->BorderWidth-1,uc->BorderWidth-1+
              TheScreen.TitleHeight,1,ShadowGC,TheScreen.blackcontext);
    DrawBevel(uc->border,xb+uc->BorderWidth,0,xb+2*uc->BorderWidth-1,\
                  uc->BorderWidth-1+TheScreen.TitleHeight,1,ShadowGC,\
                                              TheScreen.blackcontext);
    DrawBevel(uc->border,0,yb+uc->BorderWidth+TheScreen.TitleHeight,uc->BorderWidth-1,2*uc->BorderWidth-1+
              yb+TheScreen.TitleHeight,1,ShadowGC,TheScreen.blackcontext);
    DrawBevel(uc->border,xb+uc->BorderWidth,yb+uc->BorderWidth+TheScreen.TitleHeight,xb+2*uc->BorderWidth-1,\
                  yb+2*uc->BorderWidth-1+TheScreen.TitleHeight,1,ShadowGC,\
                                              TheScreen.blackcontext);
  }

  XFreeGC(disp, LightGC);
  XFreeGC(disp, ShadowGC);
}

void DrawTitle(UltimateContext *uc,char Active)
{
  XGCValues xgcv;
  GC LightGC, ShadowGC, TextGC;
  int i;

  xgcv.function=GXcopy;
  xgcv.foreground=Active
                  ? TheScreen.ActiveLight[TheScreen.desktop.ActiveWorkSpace]\
                  : TheScreen.InactiveLight[TheScreen.desktop.ActiveWorkSpace];
  xgcv.line_width=0;
  xgcv.line_style=LineSolid;
  xgcv.cap_style=CapButt;
  LightGC=XCreateGC(disp,uc->frame,GCFunction|GCForeground|\
                  GCCapStyle|GCLineWidth|GCLineStyle,&xgcv);

  xgcv.function=GXcopy;
  xgcv.foreground=Active
                  ? TheScreen.ActiveShadow[TheScreen.desktop.ActiveWorkSpace]\
                  : TheScreen.InactiveShadow[TheScreen.desktop.ActiveWorkSpace];
  xgcv.line_width=0;
  xgcv.line_style=LineSolid;
  xgcv.cap_style=CapButt;
  ShadowGC=XCreateGC(disp,uc->frame,GCFunction|GCForeground|\
                   GCCapStyle|GCLineWidth|GCLineStyle,&xgcv);
 
  xgcv.function=GXcopy;
  xgcv.foreground=Active
                  ? TheScreen.ActiveTitleFont[TheScreen.desktop.ActiveWorkSpace]
                  : TheScreen.InactiveTitleFont\
                    [TheScreen.desktop.ActiveWorkSpace];
  xgcv.font=TheScreen.TitleFont->fid;
  TextGC=XCreateGC(disp,uc->frame,GCFunction|GCForeground|GCFont,&xgcv);
 
  XClearWindow(disp,uc->title.win);

  if((i=(uc->BorderWidth-TheScreen.FrameBevelWidth-1)) <= 3) i=2;
  i=i/2+TheScreen.FrameBevelWidth;
  if(InitS.BorderTitleFlags&BT_GROOVE){
    XDrawLine(disp, uc->title.win, LightGC,
              (InitS.BorderTitleFlags&BT_CENTER_TITLE) ? 2 : 0,
	      uc->title.height-2, uc->title.width-2, uc->title.height-2);
    XDrawLine(disp, uc->title.win, LightGC, uc->title.width-2, 0,
              uc->title.width-2, uc->title.height-2);
    XDrawPoint(disp, uc->title.win, LightGC, uc->title.width-1, 0);
    if(InitS.BorderTitleFlags &  BT_CENTER_TITLE) {
      XDrawLine(disp, uc->title.win, ShadowGC,1,0,1, uc->title.height-2);
      XDrawPoint(disp, uc->title.win, LightGC, 0, 0);
    } else {
      XDrawPoint(disp, uc->title.win, LightGC, 0, uc->title.height-1);
    }
  }
  if(((TheScreen.TitleHeight + uc->BorderWidth - i) <= uc->title.height) &&
     (InitS.BorderTitleFlags & BT_LINE)) {
    XDrawLine(disp, uc->title.win, TheScreen.blackcontext,
              (InitS.BorderTitleFlags & BT_CENTER_TITLE) 
	      ? 0 : (uc->BorderWidth - i - 1),
              uc->title.height - 1, uc->title.width - 1, uc->title.height - 1);
    XDrawLine(disp, uc->title.win, TheScreen.blackcontext, uc->title.width - 1,
              uc->BorderWidth - i - 1 + TheScreen.TitleHeight,
              uc->title.width - 1, uc->title.height-1);
    if(InitS.BorderTitleFlags &  BT_CENTER_TITLE)
      XDrawLine(disp,uc->title.win, TheScreen.blackcontext, 0,
                uc->BorderWidth - i - 1 + TheScreen.TitleHeight, 0,
                uc->title.height - 1);
  }
  if(uc->title.name) XDrawString(disp, uc->title.win, TextGC,
		           (InitS.BorderTitleFlags & BT_CENTER_TITLE) ? 5 : 2,
			   TheScreen.TitleFont->ascent, uc->title.name,
			   strlen(uc->title.name));
                                               
  XFreeGC(disp, LightGC);
  XFreeGC(disp, ShadowGC);
  XFreeGC(disp, TextGC);
}
