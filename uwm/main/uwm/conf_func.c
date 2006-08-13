/***  CONF_FUNC.C: contains configuration file functions  ***/

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

#include "windows.h"
#include "workspaces.h"
#include "conf_func.h"

void cf_execute(NodeList *functions, NodeList *wins)
{
  Node *current;
  current = NULL;
  while(current = NodeNext(functions, current)) {
    cf_func_call *call;
    call = current->data;
    call->func(&(call->args), wins);
  }
}

cf_function(cf_Shell)
{
  MySystem(args->shell);
}

cf_function(cf_Quit)
{
  SeeYa(0,NULL);
}

cf_function(cf_AnyWindow)
{
  cf_execute(args->LimitWin.functions, wins);
}

cf_function(cf_NameWindow)
{
  NodeList *FoundWins;
  Node *current;

  FoundWins = NodeListCreate();
  current = NULL;
  while(current = NodeNext(wins, current)) {
    UltimateContext *uc;
    uc = current->data;
    if(!strcmp(uc->title.name, args->LimitWin.args.name)) {
      NodeAppend(FoundWins, uc);
    }
  }
  cf_execute(args->LimitWin.functions, FoundWins);
  NodeListDelete(&FoundWins);
}

cf_function(cf_AppWindow)
{
/***/fprintf(TheScreen.errout,
/***/	     "WINDOW FIND APPLICATION (cf_AppWindow) not yet implemented!\n");
}

cf_function(cf_NextWindow)
{
  NodeList *FoundWins;
  Node *current;

  FoundWins = NodeListCreate();
  current = NULL;
  while(current = NodeNext(wins, current)) {
    Node *n, *n2;
    n = n2 = InNodeList(TheScreen.UltimateList, current->data);
    do {
      if(n2 = NodeNext(TheScreen.UltimateList, n2)) break;
    } while(n != n2);
    if(n2) NodeAppend(FoundWins, n2->data);
  }
  cf_execute(args->LimitWin.functions, FoundWins);
  NodeListDelete(&FoundWins);
}

cf_function(cf_PrevWindow)
{
  NodeList *FoundWins;
  Node *current;

  FoundWins = NodeListCreate();
  current = NULL;
  while(current = NodeNext(wins, current)) {
    Node *n, *n2;
    n = n2 = InNodeList(TheScreen.UltimateList, current->data);
    do {
      if(n2 = NodePrev(TheScreen.UltimateList, n2)) break;
    } while(n != n2);
    if(n2) NodeAppend(FoundWins, n2->data);
  }
  cf_execute(args->LimitWin.functions, FoundWins);
  NodeListDelete(&FoundWins);
}

cf_function(cf_NextVisibleWindow)
{
  NodeList *FoundWins;
  Node *current;

  FoundWins = NodeListCreate();
  current = NULL;
  while(current = NodeNext(wins, current)) {
    Node *n, *n2;
    n = n2 = InNodeList(TheScreen.UltimateList, current->data);
    do {
      if((n2 = NodeNext(TheScreen.UltimateList, n2))
	 && (WinVisible(n2->data))) break;
    } while(n != n2);
    if(n2) NodeAppend(FoundWins, n2->data);
  }
  cf_execute(args->LimitWin.functions, FoundWins);
  NodeListDelete(&FoundWins);
}

cf_function(cf_PrevVisibleWindow)
{
  NodeList *FoundWins;
  Node *current;

  FoundWins = NodeListCreate();
  current = NULL;
  while(current = NodeNext(wins, current)) {
    Node *n, *n2;
    n = n2 = InNodeList(TheScreen.UltimateList, current->data);
    do {
      if((n2 = NodePrev(TheScreen.UltimateList, n2))
	 && (WinVisible(n2->data))) break;
    } while(n != n2);
    if(n2) NodeAppend(FoundWins, n2->data);
  }
  cf_execute(args->LimitWin.functions, FoundWins);
  NodeListDelete(&FoundWins);
}

cf_function(cf_NextWSWindow)
{
  NodeList *FoundWins;
  Node *current;

  FoundWins = NodeListCreate();
  current = NULL;
  while(current = NodeNext(wins, current)) {
    Node *n, *n2;
    n = n2 = InNodeList(TheScreen.UltimateList, current->data);
    do {
      if((n2 = NodeNext(TheScreen.UltimateList, n2))
	 && (OnActiveWS(((UltimateContext*)n2->data)->WorkSpace))) break;
    } while(n != n2);
    if(n2) NodeAppend(FoundWins, n2->data);
  }
  cf_execute(args->LimitWin.functions, FoundWins);
  NodeListDelete(&FoundWins);
}

cf_function(cf_PrevWSWindow)
{
  NodeList *FoundWins;
  Node *current;

  FoundWins = NodeListCreate();
  current = NULL;
  while(current = NodeNext(wins, current)) {
    Node *n, *n2;
    n = n2 = InNodeList(TheScreen.UltimateList, current->data);
    do {
      if((n2 = NodePrev(TheScreen.UltimateList, n2))
	 && (OnActiveWS(((UltimateContext*)n2->data)->WorkSpace))) break;
    } while(n != n2);
    if(n2) NodeAppend(FoundWins, n2->data);
  }
  cf_execute(args->LimitWin.functions, FoundWins);
  NodeListDelete(&FoundWins);
}

cf_function(cf_VisibleWindow)
{
  NodeList *FoundWins;
  Node *current;

  FoundWins = NodeListCreate();
  current = NULL;
  while(current = NodeNext(wins, current)) {
    UltimateContext *uc;
    uc = current->data;
    if(WinVisible(uc)) {
      NodeAppend(FoundWins, uc);
    }
  }
  cf_execute(args->LimitWin.functions, FoundWins);
  NodeListDelete(&FoundWins);
}

cf_function(cf_UniconicWindow)
{
  NodeList *FoundWins;
  Node *current;

  FoundWins = NodeListCreate();
  current = NULL;
  while(current = NodeNext(wins, current)) {
    UltimateContext *uc;
    uc = current->data;
    if(IsNormal(uc)) {
      NodeAppend(FoundWins, uc);
    }
  }
  cf_execute(args->LimitWin.functions, FoundWins);
  NodeListDelete(&FoundWins);
}

cf_function(cf_IconicWindow)
{
  NodeList *FoundWins;
  Node *current;

  FoundWins = NodeListCreate();
  current = NULL;
  while(current = NodeNext(wins, current)) {
    UltimateContext *uc;
    uc = current->data;
    if(IsIconic(uc)) {
      NodeAppend(FoundWins, uc);
    }
  }
  cf_execute(args->LimitWin.functions, FoundWins);
  NodeListDelete(&FoundWins);
}

cf_function(cf_NoneWindow)
{
  NodeList *FoundWins;
  FoundWins = NodeListCreate();
  cf_execute(args->LimitWin.functions, FoundWins);
  NodeListDelete(&FoundWins);
}

cf_function(cf_DragPos)
{
}

cf_function(cf_DragSize)
{
}

cf_function(cf_HexMenu)
{
}

cf_function(cf_SetFocus)
{
}

cf_function(cf_Show)
{
}

cf_function(cf_Raise)
{
}

cf_function(cf_Lower)
{
}

cf_function(cf_Max)
{
}

cf_function(cf_VMax)
{
}

cf_function(cf_HMax)
{
}

cf_function(cf_Demax)
{
}

cf_function(cf_Resize)
{
}

cf_function(cf_SetSize)
{
}

cf_function(cf_Repos)
{
}

cf_function(cf_SetPos)
{
}

cf_function(cf_Close)
{
}

cf_function(cf_Iconify)
{
}

cf_function(cf_Kill)
{
}

cf_function(cf_WorkspaceWin)
{
}
