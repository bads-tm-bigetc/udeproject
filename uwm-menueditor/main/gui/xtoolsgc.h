#ifndef XTOOLS_GC_H
#define XTOOLS_GC_H

#include "xobject.h"
#include "tree.h"

int xtoolsgc_cmp (void* a, void* b);
XToolsGC x_gc_create(XObject* obj, unsigned long foreground, int line_width, int line_style);
void x_gc_unref(XToolsGC gc);


#endif
