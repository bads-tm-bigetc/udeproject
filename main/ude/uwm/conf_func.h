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

union _cf_args {
  char *shell;
  NodeList *AnyWindow;  /* list of functions to execute */
  struct {
    char *name;
    NodeList *functions;  /* list of functions to execute */
  } NameWindow;
  struct {
    int x, y;
  } resize, SetSize, reposition, SetPosition;
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

#endif /* UWM_CONF_FUNC_H */
