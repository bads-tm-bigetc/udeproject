/* Test program for the new event handling metods proposed by
   Jaime Alberto <<el mono>> Silva Colorado (yes we the latinos have really
   long names) this also uses the Christian Ruppert's box concept, and also
   contains my proposed inheritance metod */

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "ude.h"

UDEDesktop desk;
UDEBoolean done= FALSE;

/* This function shows how one handler can be used for multiple boxes */
void
actions (UDEButton *but, UDEPointer data)
{
    printf ((char*)data);
}

void
where_done ()
{
    done= TRUE;
}

int
cont_func ()
{
  return !done;
}
    
int
main (int argc, char *argv[])
{
  UDEBox *box;
  UDEButton *but;
  unsigned long white_pixel;
  unsigned long black_pixel;
  int def_screen;
  
  printf("started\n");
  ude_initialize(&desk);
  printf("initialized\n");
  def_screen= DefaultScreen (desk.disp);
  black_pixel= BlackPixel (desk.disp, def_screen);
  white_pixel= WhitePixel (desk.disp, def_screen);
  box= ude_simple_box_create (&desk, NULL, 0, 0, 100, 100, 0, white_pixel,
			      white_pixel, NoEventMask, NULL);
  if(!box)
    return UDE_FAIL;
  
  but= ude_button_create (&desk, box, 10, 10, 80, 20, 2, black_pixel,
			  "blue", "green", "red");
  ude_add_event_handler ((UDEBox*) but, UDEEVENTS_GROUP, Clicked,
			 (UDEEventHandlerFunction) actions, "I'm HULK\n");
  ude_box_activate ((UDEBox*) but);
  
  but= ude_button_create (&desk, box, 10, 40, 80, 20, 2, black_pixel,
			  "blue", "yellow", "red");
  ude_add_event_handler ((UDEBox*) but,  UDEEVENTS_GROUP, Clicked,
			 (UDEEventHandlerFunction) actions, "I'm WOLVERINE\n");
  ude_box_activate ((UDEBox*) but);
  
  but= ude_button_create (&desk, box, 10, 70, 80, 20, 2, black_pixel,
			  "blue", "red", "orange");
  ude_add_event_handler ((UDEBox*) but,  UDEEVENTS_GROUP, Clicked,
			 (UDEEventHandlerFunction) actions,
			 "I'm SPIDERMAN and i'm taking you our of here\n" );
  ude_add_event_handler ((UDEBox*) but,  UDEEVENTS_GROUP, Clicked,
			 (UDEEventHandlerFunction) where_done, NULL);
  ude_box_activate ((UDEBox*) but);

  ude_box_activate (box);
  ude_handle_event_loop (&desk, cont_func);

  printf("finished\n");

  return 0;
}
