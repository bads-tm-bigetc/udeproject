/*** WINGROUPS.C: Contains routines for handling window groups ***/

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

#include "uwm.h"
#include "nodes.h"
#include "special.h"
#include "windows.h"
#include "wingroups.h"

extern Display *disp;
extern XContext UWMContext;
extern XContext UWMGroupContext;

WinGroup *CreateWinGroup(Window leader)
{
  WinGroup *group;

  group = MyCalloc(1,sizeof(*group));
  XSaveContext(disp, leader, UWMGroupContext, (XPointer) group);

  if(!(group->members = NodeListCreate()))
    SeeYa(1, "FATAL: out of memory!");

  if(XFindContext(disp, leader, UWMContext, (XPointer *)&(group->leader)))
    if(!(group->leader = UltimizeWin(leader)))
      SeeYa(1, "FATAL: out of memory!");

  RemoveWinFromGroup(group->leader);
  AddWinToGroup(group->leader);

  return group;
}

void AddWinToGroup(UltimateContext *uc)
{
  WinGroup *group;

  if(XFindContext(disp, uc->win, UWMGroupContext, (XPointer *)&group)
     && XFindContext(disp, uc->WMHints->window_group, UWMGroupContext,
                     (XPointer *)&group))
    group = CreateWinGroup(uc->WMHints->window_group);

  if(!(InNodeList(group->members, uc))) {
    if(!NodeAppend(group->members, uc)) SeeYa(1, "FATAL: out of memory");
    uc->group = group;
  }
}

/* returns -1 if whole group has been deleted, else 0 */
int RemoveWinFromGroup(UltimateContext *uc)
{
  if(!uc->group) return(0);
  if(uc == uc->group->leader) {
    if(NodeCount(uc->group->members) == 1) {
      NodeListDelete(&(uc->group->members));
      XDeleteContext(disp, uc->group->leader->win, UWMGroupContext);
      free(uc->group);

      uc->group = NULL;
      return(-1);
    } else {
      Node2End(uc->group->members, InNodeList(uc->group->members, uc));
    }
  } else {
    NodeDelete(uc->group->members, InNodeList(uc->group->members, uc));
    uc->group = NULL;
  }
  return(0);
}

void DeleteWinGroup(WinGroup *group)
{
  Node *n;
   
  while(-1 != RemoveWinFromGroup(NodeNext(group->members, NULL)->data));
}

void UpdateWinGroup(UltimateContext *uc)
{
  if(uc->WMHints && (uc->WMHints->flags & WindowGroupHint)) {
    if(uc->group && (uc->group->leader->win == uc->WMHints->window_group))
      return;
    RemoveWinFromGroup(uc);
    AddWinToGroup(uc);
  } else RemoveWinFromGroup(uc);
}
