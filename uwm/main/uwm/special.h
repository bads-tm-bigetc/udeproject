/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifndef UWM_SPECIAL_H
#define UWM_SPECIAL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include "uwm.h"

void UpdateDesktop();
void ShowMenu(int menuNumber, int x, int y);
void BorderButton(char a,UltimateContext *uc,int x,int y,int x_root,int y_root);
void SendWMProtocols(UltimateContext *uc, Atom prot);
void SendConfigureEvent(UltimateContext *uc);
void SendSelectionNotify(XEvent *request, Atom property);
int ChangeProperty(Window win, Atom property, Atom type, int format, int mode, 
                   char *data, int nelements);
void GrabPointer(Window win,unsigned int mask,Cursor mouse);
void UngrabPointer();
void GrabServer();
void UngrabServer();
void RaiseWin(UltimateContext *uc);
void LowerWin(UltimateContext *uc);
pid_t MySystem(char *command);
char *MyStrdup(char *s);
void *MyCalloc(size_t n,size_t s);
FILE *MyOpen(char *name, char *ppopts);
int CheckCPP();

#endif
