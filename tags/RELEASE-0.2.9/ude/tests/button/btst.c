#include <X11/Xlib.h>
#include <X11/xpm.h>

#include "ude.h"
#include "ude-button.h"
#include "ude-hex.h"

UDEDesktop desk;
UDEButton *button1, *button2;
UDEBox *box;

static int ex = -1;

void exi(UDEBox *box, UDEPointer data, XEvent *event)
{
  ex = 0;
}


void test(UDEBox *box, UDEPointer data, XEvent *event)
{
  printf("%s\n",data);
}


int cf()
{
  printf("%d\n",button1->flags & UDEButtonDown);
  return ex;
}

main()
{
  int x,y,w,h;
  Pixmap pm,ps,pa,pmo;
  XpmAttributes xa;

  if(ude_initialize(&desk) != UDE_SUCCESS) {
    printf("either no uwm or uds running or couldn't open display.\nexiting\n");
    exit(1);
  }

  box = ude_simple_box_create(&desk, NULL, 0, 0, 200, 200, 0, 0,
                              desk.ActualColors[UDE_Back].pixel, 0, NULL);
  if(!box) {
    printf("no box.\n");
    exit(1);
  }

  xa.valuemask = 0;
  XpmReadFileToPixmap(desk.disp, box->win,
                      "/home/arc/udegfx/icons/Ude-base.xpm",
                      &ps, &pm, &xa);
  xa.valuemask = 0;
  XpmReadFileToPixmap(desk.disp, box->win,
                      "/home/arc/udegfx/icons/ude-computer.xpm",
                      &pa, &pm, &xa);
  xa.valuemask = 0;
  XpmReadFileToPixmap(desk.disp, box->win,
                      "/home/arc/udegfx/icons/ude-trash.xpm",
                      &pmo, &pm, &xa);

  w = xa.width; h = xa.height;
//  ude_calc_hex_measures(&h, &w, UDEHexFitInside);
  x = 100 - w/2; y = 100 - h/2;
  button1 = ude_button_create(&desk, box, "test 1", x, y, w, h, ps, pa,
                              pmo, pm, /*UDEButtonUserPixmaps|*/UDEButtonShaped|UDEButtonSwitch,
                              NULL, NULL);
  if(!button1) {
    printf("no button1.\n");
    exit(1);
  }
  ude_box_activate(&button1->box);
  ude_add_event_handler (&button1->box, UDEEVENTS_GROUP, UDEEAction, test,
                         "test 1 activated.");
  ude_add_event_handler (&button1->box, UDEEVENTS_GROUP, UDEETouched, test,
                         "test 1 touched.");

  ude_hex_coords(w, 50, h, &x, &y, UDEHexLeft, UDEHexAbove);

  button2 = ude_button_create(&desk, box, "exit", x, y, 50, h, None, None,
                             None, None, UDEButtonShaped, NULL, NULL);
  if(!button2) {
    printf("no button2.\n");
    exit(1);
  }
  ude_box_activate(&button2->box);
  ude_add_event_handler (&button2->box, UDEEVENTS_GROUP, UDEEAction, exi, NULL);
  ude_add_event_handler (&button2->box, UDEEVENTS_GROUP, UDEETouched, test,
                         "exit button touched.");

  ude_box_activate(box);
  ude_handle_event_loop(&desk, cf);
}
