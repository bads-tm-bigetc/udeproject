/***  WINMENUMENU.C: Contains routines for the window menu  ***/

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

#include <stdlib.h>

#include "uwm.h"
#include "special.h"
#include "menu.h"
#include "workspaces.h"
#include "settings.h"
#include "uwm_intl.h"

extern UDEScreen TheScreen;

typedef struct {
  void *Proc;
  short arg;
} Procs;

Procs *wsprocs;

void InitWSProcs()
{
  int a;

  wsprocs = MyCalloc(NUMBER_OF_WORKSPACES, sizeof(Procs));
  for(a = 0; a < NUMBER_OF_WORKSPACES; a++) {
    wsprocs[a].Proc = WithWin2WS;
    wsprocs[a].arg = a;
  }
}

void Sticky(UltimateContext *uc,short dummy)
{
  StickyWin(uc);
}
Procs sticky={Sticky,0};

void WinMenuMenu(UltimateContext *TheWin,int x, int y)
{
  int a;
  Menu *men,*wsmen;
  MenuItem *item;
  short useTitle = (settings.global_settings->LayoutFlags & SUBMENU_TITLES);

  if(!(men = TheWin->title.name ? MenuCreate(TheWin->title.name) : MenuCreate("")))
    SeeYa(1,"FATAL: out of memory!");

  if(NUMBER_OF_WORKSPACES > 1)
    {
      AppendMenuItem(men, _("Sticky window"), &sticky,\
		     (TheWin->WorkSpace == -1) ? I_SWITCH_ON : I_SWITCH_OFF);

      wsmen = MenuCreate((TheWin->title.name && useTitle)
                         ? TheWin->title.name : NULL);
      if(!wsmen)
	SeeYa(1,"FATAL: out of memory!");
      for (a=0; a < NUMBER_OF_WORKSPACES; a++)
	   AppendMenuItem (wsmen, settings.workspace_settings[a]->Name,
			   &wsprocs[a], I_SELECT);
      AppendMenuItem (men, _("Move to WorkSpace"), wsmen, I_SUBMENU);
    }

/*  AppendMenuItem(men,TheScreen.Texts.WinMenuMenu.AlwaysOnTop,NULL,I_SELECT);*/

  if(item=StartMenu(men,x,y,True,NULL)){
    if((item->type==I_SELECT)||(SWITCHTYPE(item->type))){
      Procs *pr;
      if(pr=item->data){
        HandlerTable p;
        p=pr->Proc;
        p(TheWin,pr->arg);
      }
    }
  }
  DestroyMenu(men);
}

