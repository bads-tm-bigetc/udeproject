/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifndef UWM_CONF_FUNC_H
#define UWM_CONF_FUNC_H

#include "special.h"
#include "nodes.h"

/* datatypes */
typedef struct _cf_func_call cf_func_call;
typedef union _cf_args cf_args;
typedef int (*cf_func)(cf_args *args, NodeList *wins);

#define CF_MODE_WIN_LIMIT 0
#define CF_MODE_WIN_FIND  1

union _cf_args {
  char *shell;
  struct {
    NodeList *functions;		/* list of functions to execute */
    cf_func_call *else_function;	/* function to execute if no match */
    int mode;				/* CF_MODE_WIN_... */
    union {
      char *name, *application;
    } args;
  } LimitWin;
  struct {
    int x, y;
  } Resize, SetSize, Repos, SetPos;
};

struct _cf_func_call {
  cf_func func;
  cf_args args;
};

/* Prototypes */
#define cf_function(A) int A(cf_args *args, NodeList *wins)
extern cf_function(cf_Shell);
extern cf_function(cf_Quit);
extern cf_function(cf_AnyWindow);
extern cf_function(cf_NameWindow);
extern cf_function(cf_AppWindow);
extern cf_function(cf_NextWindow);
extern cf_function(cf_PrevWindow);
extern cf_function(cf_NextVisibleWindow);
extern cf_function(cf_PrevVisibleWindow);
extern cf_function(cf_NextWSWindow);
extern cf_function(cf_PrevWSWindow);
extern cf_function(cf_VisibleWindow);
extern cf_function(cf_UniconicWindow);
extern cf_function(cf_IconicWindow);
extern cf_function(cf_NoneWindow);
extern cf_function(cf_DragPos);
extern cf_function(cf_DragSize);
extern cf_function(cf_HexMenu);
extern cf_function(cf_SetFocus);
extern cf_function(cf_Show);
extern cf_function(cf_Raise);
extern cf_function(cf_Lower);
extern cf_function(cf_Max);
extern cf_function(cf_VMax);
extern cf_function(cf_HMax);
extern cf_function(cf_Demax);
extern cf_function(cf_Resize);
extern cf_function(cf_SetSize);
extern cf_function(cf_Repos);
extern cf_function(cf_SetPos);
extern cf_function(cf_Close);
extern cf_function(cf_Iconify);
extern cf_function(cf_Kill);
extern cf_function(cf_WorkspaceWin);

#endif /* UWM_CONF_FUNC_H */
