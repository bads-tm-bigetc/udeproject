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
  struct {
    char *s;
  } shell;
  struct {
    int x, y;
  } resize, SetSize, reposition, SetPosition;
};

struct _cf_func_call {
  cf_func func;
  cf_args args;
};

/* Prototypes */
#define cf_function(A) int *A(cf_args *args, NodeList *wins)
extern cf_function(cf_Shell);

#endif /* UWM_CONF_FUNC_H */
