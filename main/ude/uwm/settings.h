/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifndef UWM_SETTINGS_H
#define UWM_SETTINGS_H

#include <X11/xpm.h>
#include "ude_config_consts.h"
#include "nodes.h"
#include "tree.h"

typedef struct _FontStruct {
  XFontStruct *xfs;
  char *name;
} FontStruct;

/* global settings */
typedef struct _uwm_global_settings {
#define GO__STRUCT
#include "settings_global_list.h"
} uwm_global_settings;

typedef struct _uwm_image {
  Pixmap image;
  XpmAttributes *attributes;
} uwm_image;

/* workspace specific settings */
typedef struct _uwm_workspace_settings {
#define WO__STRUCT
#include "settings_workspace_list.h"
} uwm_workspace_settings;

/* Contexts: */
#define EventRootContext (0)
#define EventWindowContext (1)
#define EventMenuContext (2)
#define EventMenuItemContext (3)
#define EventWinMenuItemContext (4)

typedef struct _uwm_button_event {
  struct {
    unsigned int context;
    unsigned int state;
    unsigned int button;
  } event;
  NodeList program;
} uwm_button_event;

typedef struct _uwm_key_event {
  struct {
    unsigned int context;
    unsigned int state;
    KeySym keysym;
  } event;
  NodeList program;
} uwm_key_event;

typedef struct _uwm_user_events {
  struct {
    NodeList *list;
    TreePot *tree;
  } uwm_key_events;
  struct {
    NodeList *list;
    TreePot *tree;
  } uwm_button_events;
} uwm_user_events;

typedef struct _uwm_settings {
  uwm_global_settings *global_settings;
  int workspace_settings_count;
  uwm_workspace_settings **workspace_settings;
  uwm_user_events *user_events;
} uwm_settings;

extern uwm_settings settings;

#endif
