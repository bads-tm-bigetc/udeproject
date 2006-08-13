#include "ude.h"

UDEDesktop desk;

main()
{
  int a,b;
  ude_initialize(&desk);
  printf("UDE desktop data viewer 1999 by Christian Ruppert\n\n");
  printf("Display: %d\n",desk.disp);
  printf("Workspaces: %d\n",desk.WorkSpaces);
  printf("Active Workspace: %d\n",desk.ActiveWorkSpace);
  printf("Frame Bevel Width: %d\n",desk.FrameBevelWidth);
  printf("Flags: %d\n",desk.flags);
  printf("Standard Font: %s\n",desk.StandardFont);
  printf("Inactive Font: %s\n",desk.InactiveFont);
  printf("Highlight Font: %s\n",desk.HighlightFont);
  printf("Text Font: %s\n",desk.TextFont);
  for(a=0;a<desk.WorkSpaces;a++) {
    printf("\nWorkspace %d - %s:\n",a,desk.WorkSpacesData[a].name);
    for(b=0;b<UDE_MAXCOLORS;b++) {
      printf("%d, %d, %d - ",desk.WorkSpacesData[a].WorkspaceColors[b].red,
                             desk.WorkSpacesData[a].WorkspaceColors[b].green,
                             desk.WorkSpacesData[a].WorkspaceColors[b].blue);
    }
    printf("\n");
  }
}
