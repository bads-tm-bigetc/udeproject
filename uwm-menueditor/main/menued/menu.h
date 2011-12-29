#ifndef MENU_H_
#define MENU_H_

#include "list.h"
#include "stdio.h"

#define MENUNODECNT 7
#define MENUKEYS 6
#define GAP "  "

typedef enum {
	SUBMENU,
	ITEM,
	LINE,
	FILE_KEY,
	PIPE,
	PAREN_CLOSE,
	DUMMY
} MenuNodeType;

typedef struct MENUNODE  menunode;
typedef struct MENUNODE* pmenunode;

struct MENUNODE
{
	MenuNodeType type;
	plist childlist;
	pmenunode parent;
	char *info1, *info2;
};

pmenunode menunode_allocate();
void menunode_print(FILE* out, pmenunode cur, int depth);
pmenunode menunode_read(FILE *in);
void menunode_destroy(pmenunode cur);

#endif
