/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifndef UWM_CONF_FUNC_H
#define UWM_CONF_FUNC_H

/* datatypes */
#define CFD_VOID -1
#define CFD_INT 0
#define CFD_STRING 1
#define CFD_FUNC 2
typedef struct _cf_data {
  int type;
  union _cf_data_data {
    int i;
    char *s;
    struct _cf_func_def *f;
  } data;
} cf_data;

typedef struct _cf_data_array {
  int nelem;
  cf_data *data;
} cf_data_array;

/* general function type */
typedef cf_data *(*cf_func)(cf_data_array *args);

/* function definition structure */
typedef struct _cf_func_def {
  int type;
  cf_func func;
  char *name;
} cf_func_def;

typedef struct _cf_func_call {
  cf_func func;
  cf_data_array args;
} cf_func_call;

/* Prototypes */
#define CF__PROTOTYPES
#include "conf_func_list.h"

extern const cf_func_def cf_functions[CONFIG_FUNC_COUNT];

#endif /* UWM_CONF_FUNC_H */
