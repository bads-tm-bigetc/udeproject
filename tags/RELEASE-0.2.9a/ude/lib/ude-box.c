/* licenced under GPL, (c) Christian Ruppert */

/* TODOS:
   * add and support a children's list for each box. we can use the uwm
     nodes-module for that.
   * do refreshs in case of a workspace switch
   * set a box's background color automatically to the selected background color
     if not overriden.
   * add a ude default handler 'refresh' which will (re-) draw the box.
*/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "ude-box.h"

/******************* PRIVATE: DO NOT USE THIS EXTERNALLY ******************/
typedef struct _UDEHandler UDEHandler;
typedef struct _UDEEventHandlers UDEEventHandlers;

struct _UDEHandler {
  UDEEventHandlerFunction handler;
  UDEPointer data;
};

struct _UDEEventHandlers {
  int event_type;
  UDEVector *handlers;
};

int
ude_event_handlers_compare (UDEEventHandlers *h1, UDEEventHandlers *h2)
{
  if (h1->event_type > h2->event_type)
    return 1;
  else if (h1->event_type < h2->event_type)
    return -1;
  return 0;
}

void
ude_event_handlers_free (UDEEventHandlers *h)
{
  ude_vector_free (h->handlers, TRUE);
  free (h);
}

inline UDEBox*
ude_box_init_1 (UDEDesktop *desk, UDEBox *parent, UDEPointer UserData,
		Window *parentWin)
{
  UDEBox *box= (UDEBox*) malloc (sizeof (UDEBox));
  
  if (!box)
    return NULL;
  box->desktop = desk;
  box->parent = parent;
  box->UserData = UserData;
  box->x_handlers= NULL;
  box->ude_handlers= NULL;
  *parentWin = parent ? parent->win :
    RootWindow (desk->disp, DefaultScreen(desk->disp));
  return box;
}

void
_UDE_ConfigureNotify_handler (UDEBox *box, UDEPointer data, XEvent *event)
{
  XGetWindowAttributes (box->desktop->disp, box->win, &box->attributes);
}

inline UDEBox*
ude_box_init_2 (UDEBox *box)
{
  if (XSaveContext (box->desktop->disp, box->win, UDEContext,
		    (XPointer)box))
    {
      XDestroyWindow (box->desktop->disp, box->win);
      free (box);
      return NULL;
    }
  /*
  if (!(box->parent))
    {
      box's window is top-level window -> act icccm-compliant
	 (not supported yet)
    }
   */
  ude_add_event_handler (box, XEVENTS_GROUP, ConfigureNotify,
                         _UDE_ConfigureNotify_handler, NULL);
  XGetWindowAttributes (box->desktop->disp, box->win, &box->attributes);
  return box;
}


/***************************************************************************/

/* creates a box and returns the corresponding values in the given box
   structure. 
   return values: ==NULL - box couldn't be created
                  !=NULL - creation successful: pointer to created box */
UDEBox*
ude_box_create (UDEDesktop *desk, UDEBox *parent, int x, int y,
		unsigned int width, unsigned int height,
		unsigned int border_width, int depth,
		Visual *visual, XSetWindowAttributes *winattributes,
		unsigned long valuemask, UDEPointer UserData)
{
  UDEBox *box;
  Window parentWin;

  box= ude_box_init_1 (desk, parent, UserData, &parentWin);
  if(!box)
    return NULL;
  if(winattributes) winattributes->event_mask |= UDE_all_boxes_events_mask;
  box->win = XCreateWindow (desk->disp, parentWin, x, y, width, height,
			    border_width, depth, InputOutput, visual,
			    valuemask, winattributes);
  return (ude_box_init_2 (box));
}

/* creates a simple box and returns the corresponding values in the given box
   structure. 
   return values: ==NULL - box couldn't be created
                  !=NULL - creation successful: pointer to created box */
UDEBox*
ude_simple_box_create (UDEDesktop *desk, UDEBox *parent, int x, int y, 
		       unsigned int width, unsigned int height,
		       unsigned int border_width, unsigned long border,
		       unsigned long background,
		       long event_mask, UDEPointer UserData)
{
  UDEBox *box;
  Window parentWin;

  box= ude_box_init_1 (desk, parent, UserData, &parentWin);
  if(!box)
    return NULL;
  event_mask |= UDE_all_boxes_events_mask;
  box->win = XCreateSimpleWindow (desk->disp, parentWin, x, y, width, height,
				  border_width, border, background);
  XSelectInput (desk->disp, box->win, event_mask);
  return (ude_box_init_2 (box));
}

void
ude_box_activate (UDEBox *box)
{
  XMapWindow (box->desktop->disp, box->win);
  XFlush (box->desktop->disp);
}

void
ude_box_deactivate (UDEBox *box)
{
  XUnmapWindow (box->desktop->disp, box->win);
}

void
ude_box_destroy (UDEBox *box)
{
  ude_box_deactivate (box);
  XDeleteContext (box->desktop->disp, box->win, UDEContext);
  XDestroyWindow (box->desktop->disp, box->win);
  ude_vector_for_each (box->x_handlers, ude_event_handlers_free);
  ude_vector_free (box->x_handlers, FALSE);
  ude_vector_for_each (box->ude_handlers, ude_event_handlers_free);
  ude_vector_free (box->ude_handlers, FALSE);
  free (box);
}

inline UDEEventHandlers*
get_handlers (UDEBox *box, UDEEventGroup event_group, int event_type)
{
  UDEEventHandlers *handlers, srch;
  UDEVector **handlers_vector;

  /* This could be changet to a switch in case that more grops be added
     but i don't think that will happend */
  if (event_group == UDEEVENTS_GROUP)
    handlers_vector= &(box->ude_handlers);
  else /* event_group == XEVENTS_GROUP */
    handlers_vector= &(box->x_handlers);

  if (*handlers_vector)
    {
      srch.event_type= event_type;
      handlers= ude_vector_find (*handlers_vector, &srch);
    }
  else
    {
      handlers= NULL;
      *handlers_vector= ude_vector_create (TRUE,
					   (UDECompareFunction)
					   ude_event_handlers_compare);
    }
  if (!handlers)
    {
      handlers= (UDEEventHandlers*) malloc (sizeof (UDEEventHandlers));
      if (!handlers)
	return NULL;
      handlers->event_type= event_type;
      handlers->handlers= ude_vector_create (FALSE, NULL);
      ude_vector_add (*handlers_vector, handlers);
    }
  return handlers;
}
  
void
ude_add_event_handler (UDEBox *box,
		       UDEEventGroup event_group, int event_type,
		       UDEEventHandlerFunction function, UDEPointer data)
{
  UDEEventHandlers *handlers;
  UDEHandler *new_handler;

  handlers= get_handlers (box, event_group, event_type);
  /* The function is added regardless if it has been added before or not.
     The functions have no order criteria, they are puted in the same order
     they arrive so searching one by one every time a function is added will
     take too mouch time and it could be nice to be able to call an event
     handler more than one time for event */
  new_handler= (UDEHandler*) malloc (sizeof (UDEHandler));
  if (!new_handler)
    return;
  new_handler->handler= function;
  new_handler->data= data;
  ude_vector_add (handlers->handlers, new_handler);
}

void
ude_add_event_handler_first (UDEBox *box, 
			     UDEEventGroup event_group, int event_type,
			     UDEEventHandlerFunction function,
			     UDEPointer data)
{
  UDEEventHandlers *handlers;
  UDEHandler *new_handler;

  handlers= get_handlers (box, event_group, event_type);
  new_handler= (UDEHandler*) malloc (sizeof (UDEHandler));
  if (!new_handler)
    return;
  new_handler->handler= function;
  new_handler->data= data;
  ude_vector_set_nth (handlers->handlers, new_handler, 0);
}

void
ude_remove_event_handler (UDEBox *box,
			  UDEEventGroup event_group, int event_type,
			  UDEEventHandlerFunction function)
{
  /* Sorry but the functions have to be searched one by one */
  UDEEventHandlers *hs, srch;
  UDEHandler *h;
  size_t cont;
  UDEBoolean founded= FALSE;
  UDEVector *handlers_vector;

  /* Set the vector that will be modified */
  if (event_group == UDEEVENTS_GROUP)
    handlers_vector= box->ude_handlers;
  else
    handlers_vector= box->x_handlers;
  
  if (!handlers_vector)
    return;
  srch.event_type= event_type;
  hs= ude_vector_find (handlers_vector, &srch);
  if (!hs)
    return;
  for (cont= 0; cont < hs->handlers->length; cont++)
    {
      h= ude_vector_get_nth (hs->handlers, cont);
      if (function == h->handler )
	{
	  founded= TRUE;
	  break;
	}
    }
  if (founded)
    ude_vector_delete_element (hs->handlers, cont, TRUE);
}

void
ude_send_ude_event (UDEBox *box, int ude_event_type)
{
 UDEEventHandlers *hs, srch;
 UDEHandler *h;
 size_t cont;

 if (box->ude_handlers)
   {
     srch.event_type= ude_event_type;
     hs= ude_vector_find (box->ude_handlers, &srch);
     if (hs)
       for (cont= 0; cont < (hs->handlers->length); cont++)
	 {
	   h= ude_vector_get_nth (hs->handlers, cont);
	   h->handler (box, h->data, NULL);
	 }
   }
}

void
ude_handle_event_loop (UDEDesktop *desk, int (*cont_function)())
{
  XEvent event;
  UDEBox *box;
  UDEEventHandlers *hs, srch;
  UDEHandler *h;
  size_t cont;
  
  while (cont_function ())
    {
      XNextEvent (desk->disp, &event);
      if((event.xany.type == PropertyNotify) 
         && (event.xany.window
             == RootWindow(desk->disp,DefaultScreen(desk->disp))))
      {
        if (event.xproperty.atom == UDE_WORKSPACES_PROPERTY)
          ude_update_UDEWorkspaceExchange(desk);
        if (event.xproperty.atom == UDE_SETTINGS_PROPERTY)
          ude_update_UDEDesktop(desk);
      }
      if((!XFindContext(desk->disp, event.xany.window, UDEContext,
		        (XPointer *)&box)) && (box->x_handlers))
      {
	srch.event_type= event.type;
	hs= ude_vector_find (box->x_handlers, &srch);
	if (hs)
	  for (cont= 0; cont < (hs->handlers->length); cont++)
	  {
	    h= ude_vector_get_nth (hs->handlers, cont);
	    h->handler (box, h->data, &event);
	  }
      }
    }
}

