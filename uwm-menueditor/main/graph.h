#ifndef GRAPH_H_
#define GRAPH_H_

#include "menu.h"
#include "gui/xtools.h"
#include "special.h"

typedef struct pathstruct pathstruct;
typedef struct pathstruct* ppathstruct;

struct pathstruct
{
	char* filepath;
	pmenunode cur;
	pmenunode root;
	Bool changed;
};

pathstruct* pathstruct_create(char* filepath, pmenunode cur, Bool changed);
Bool quit(XObject* obj, XEvent ev, XPointer cbdata);
Bool hide_window(XObject* obj,XEvent ev, XPointer cbdata);
Bool leave_submenu(XObject* obj,XEvent ev, XPointer cbdata);
Bool create_object(XObject* obj,XEvent ev,XPointer cbdata);
void appendsubmenutolist(plist l,plist ab, plist menu);
void fill_window(pmenunode top,char* path);
void clearmenulist(plist l);
void update_currentlist();
void update_name(XObject* obj, XPointer cbdata);
void update_command(XObject* obj, XPointer cbdata);
void appendnodetolist(plist l, plist ab, pnode it);
Bool item_press(XObject* obj,XEvent ev,XPointer cbd);
Bool resize(XObject* obj,XEvent e, void* cbdata);
#endif
