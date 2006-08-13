#include <X11/Xlib.h>
#include <X11/xpm.h>
#include <stdio.h>

#include "ude.h"
#include "ude-menu.h"

#define	MENU1_NUMITEMS	7

UDEDesktop desk;
UDEBox *box;
UDEMenu *menu1, *menu2;
static int cont = 1;

int loop()
{
  printf("The Switch is %s\n", (menu1->menu_item[3]->flags & UDEMenuItemDown) ?
  							"down." : "up.");
  return cont;
}

void quit(UDEBox *box, UDEPointer data, XEvent *event)
{
  printf("Preparing to quit...\n");
  cont = 0;
}

void tell(UDEBox *box, UDEPointer data, XEvent *event)
{
    printf("%s was %s.\n", ((UDEMenuItem *)box)->name, (char *)data);

  return;
}

void help(UDEBox *box, UDEPointer data, XEvent *event)
{
  printf("This is not a helpful button,\n");
  printf("but it could diplay helpful info in a status bar...\n");

  return;
}
int main()
{
  UDEMenuItem **item;
  int i;

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

  XStoreName(desk.disp, box->win, "Menu Test 01");

  item = malloc(MENU1_NUMITEMS * sizeof(UDEMenuItem *));
  for(i = 0; i < MENU1_NUMITEMS; i++) item[i] = malloc(sizeof(UDEMenuItem));

/* There should be a better way to do the following but I just don't know...
 * Maybe a _ude_add_menuitem function but I really don't like this kind of
 * functions. They are sooo slow... :) */
  item[0]->flags = UDEMenuItemSelectable;
  item[0]->name = "Simple Item 1";
  item[1]->flags = UDEMenuItemSelectable;
  item[1]->name = "Simple Item 2";
  item[2]->flags = 0;
  item[2]->name = "Disabled Item";
  item[3]->flags = UDEMenuItemSelectable|UDEMenuItemSwitch;
  item[3]->name = "Switchable Item";
  item[4]->flags = UDEMenuItemSelectable;
  item[4]->name = "Helpful item!";
  item[5]->flags = UDEMenuItemSelectable;
  item[5]->name = "Quit";
  item[6] = NULL;
  menu1 = ude_menu_create(&desk, box, 10, 10, "Menu1", 0, None, item, NULL);

  if(!menu1) {
    printf("no menu1.\n");
    exit(1);
  }
  ude_box_activate(&menu1->box);
  
  ude_add_event_handler(&(menu1->menu_item[0]->box), UDEEVENTS_GROUP,
                                                 UDEEAction, tell, "activated");
  ude_add_event_handler(&(menu1->menu_item[0]->box), UDEEVENTS_GROUP,
                                                 UDEETouched, tell, "touched");
  ude_add_event_handler(&(menu1->menu_item[1]->box), UDEEVENTS_GROUP,
                                                 UDEEAction, tell, "activated");
  ude_add_event_handler(&(menu1->menu_item[1]->box), UDEEVENTS_GROUP,
                                                 UDEETouched, tell, "touched");
  /*Nothing should happen with the one bellow*/
  ude_add_event_handler(&(menu1->menu_item[2]->box), UDEEVENTS_GROUP,
                                                 UDEEAction, quit, NULL);
  ude_add_event_handler(&(menu1->menu_item[3]->box), UDEEVENTS_GROUP,
                                                 UDEETouched, tell, "touched");
  ude_add_event_handler(&(menu1->menu_item[4]->box), XEVENTS_GROUP,
                                                 EnterNotify, help, NULL);
  ude_add_event_handler(&(menu1->menu_item[5]->box), UDEEVENTS_GROUP,
                                                 UDEEAction, quit, NULL);

  ude_box_activate(box);
  ude_handle_event_loop(&desk, loop);
  
  printf("Destroying boxes...\n");
  ude_menu_destroy(menu1);
  ude_box_destroy(box);

  return 0;
}
