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
  cf_execute(args->AnyWindow, wins);
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
    if(!strcmp(uc->title.name, args->NameWindow.name)) {
      NodeAppend(FoundWins, uc);
    }
  }
  cf_execute(args->NameWindow.functions, FoundWins);
  NodeListDelete(&FoundWins);
}

cf_function(cf_AppWindow)
{
}

cf_function(cf_NextWindow)
{
}

cf_function(cf_PrevWindow)
{
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
  cf_execute(args->NameWindow.functions, FoundWins);
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
  cf_execute(args->NameWindow.functions, FoundWins);
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
  cf_execute(args->NameWindow.functions, FoundWins);
  NodeListDelete(&FoundWins);
}

cf_function(cf_NoneWindow)
{
  NodeList *FoundWins;
  FoundWins = NodeListCreate();
  cf_execute(args->NameWindow.functions, FoundWins);
  NodeListDelete(&FoundWins);
}

