/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifndef UWM_URM_H
#define UWM_URM_H

#define MAXEXTRAS (17)
#define BACKGROUND (0)
#define LIGHTCOLOR (1)
#define SHADOWCOLOR (2)
#define STANDARDTEXT (3)
#define INACTIVETEXT (4)
#define INACTIVEBGR (5)
#define HIGHLIGHTEDTEXT (6)
#define HIGHLIGHTEDBGR (7)
#define TEXTCOLOR (8)
#define TEXTBGR (9)
#define BEVELWIDTH (10)
#define BEH_FLAGS (11)
#define LAY_FLAGS (12)
#define STANDARDFONT (13)
#define INACTIVEFONT (14)
#define HIGHLIGHTFONT (15)
#define TEXTFONT (16)

typedef struct _UDEXrdbEntry UDEXrdbEntry;

struct _UDEXrdbEntry {
  char *name, *entry;
  int xtra;
  struct _UDEXrdbEntry *next;
};

int ReadResourceDBFromFile(char *filename);
void SetResourceDB();
char *Initurdbcppopts();

#endif
