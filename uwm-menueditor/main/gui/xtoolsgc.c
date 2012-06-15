#include "tree.h"
#include "xtoolsgc.h"
#include "xobject.h"
#include <stdlib.h>
#include <stdio.h>

extern Display* dis;
extern int screen;
extern Tree gctree;

int xtoolsgc_cmp (void *a, void *b)
{
  XGCInfo *pa=(XGCInfo*)a, *pb=(XGCInfo*)b;
  if( pa->foreground != pb->foreground )
    {
      if(pa->foreground < pb->foreground)
	return -1;
      return 1;
    }
  if(pa->line_width != pb->line_width)
    {
      if(pa->line_width < pb->line_width)
	return -1;
      return 1;
    }
  if(pa->line_style < pb->line_style)
    return -1;
  else if( pa->line_style == pb->line_style )
    return 0;
  else
    return 1;
}

XToolsGC x_gc_create(XObject* obj, unsigned long foreground, int line_width, int line_style)
{
  XGCInfo a;
  XGCValues val;
  XToolsGC* res;
  TreapNode * cur;

  a.foreground = foreground;
  a.line_width = line_width;
  a.line_style = line_style;
  
  cur = tree_find_node( &gctree, (void*) &a );
  if( cur != NULL)
    {
      res = (XToolsGC*) (cur -> data);
      res->dat.count++;
      return *res;
    }
  
  val.foreground = foreground;
  val.line_width = line_width;
  val.line_style = line_style;
  
  res = (XToolsGC*) malloc( sizeof(XToolsGC) );
  if( res == NULL)
    {
      fprintf(stderr,"Failed to create graphic context: Out of memory\n");
      exit(OUT_OF_MEMORY);
    }
  res->inf = a;
  res->dat.count = 1;
  res->dat.gc = XCreateGC(dis, obj->obj.win,GCForeground|GCLineWidth|GCLineStyle,&val);

  tree_insert( &gctree, &res->inf, res );
  return *res;
}

void x_gc_unref(XToolsGC gc)
{
  TreapNode * cur;
  cur = tree_find_node( &gctree, &(gc.inf) );
  if(cur==NULL)
  {
      fprintf(stderr, "Warning: gc is already freed\n");
  }
  else
  {
    ((XToolsGC*)(cur->data))->dat.count--;
    if(((XToolsGC*)(cur->data))->dat.count==0)
      {
        void *t1, *t2;
        tree_erase( &gctree, &(gc.inf), &t1, &t2);
        XFreeGC(dis, gc.dat.gc);
        free(t2);
      }
  }
}
