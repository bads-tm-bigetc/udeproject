#ifndef UWM_SETTINGS_H
#define UWM_SETTINGS_H

#include <X11/xpm.h>

typedef struct _FontStruct {
  XFontStruct *xfs;
  char *name;
} FontStruct;

/* global settings */
typedef struct _uwm_global_settings {
/*** geometry */
  int BorderWidth;
  int TransientBorderWidth;
  int TitleHeight;
  int FrameBevelWidth;

  int BevelWidth;
  int MenuXOffset, MenuYOffset;
  int FrameFlags;

/*** fonts */
  FontStruct TitleFont;
  FontStruct Font;
  FontStruct MonoFont;
  FontStruct HighlightFont;
  FontStruct InactiveFont;

/*** batch */
  char *StartScript;
  char *StopScript;
  char *ResourceFile;
  char *HexPath;

/*** behaviour */
  int PlacementStratety;
  int PlacementThreshold;
  int OpaqueMoveSize;

  int MaxWinWidth;
  int MaxWinHeight;

  double WarpPointerToNewWinH;
  double WarpPointerToNewWinV;

  int SnapDistance;
  int BehaviourFlags;

/*  void *ButtonsOnRoot;  /* to be implemented/changed */
/*  void *ButtonsOnWin;   /* to be implemented/changed */
} uwm_global_settings;

/* workspace specific settings */
typedef struct _uwm_workspace_settings {
/*** general workspace options */
  char *Name;
  char *ScreenCommand;
  struct {
    Pixmap *image;
    XpmAttributes *attributes;
  } Wallpaper;

/*** colors */
  XColor *ScreenColor;
  XColor *InactiveColor, *InactiveLight, *InactiveShadow;
  XColor *InactiveTitle;
  XColor *ActiveColor, *ActiveLight, *ActiveShadow;
  XColor *ActiveTitle;

  XColor *BackgroundColor, *BackgroundLight, *BackgroundShadow;
  XColor *ForegroundColor;
  XColor *InactiveForeground;
  XColor *InactiveBackground;
  XColor *HighlightedForeground;
  XColor *HighlightedBackground;
  XColor *TextForeground;
  XColor *TextBackground;
} uwm_workspace_settings;

typedef struct _uwm_settings {
  uwm_global_settings *global_settings;
  int workspace_settings_count;
  uwm_workspace_settings **workspace_settings;
} uwm_settings;

extern uwm_settings settings;

#endif
