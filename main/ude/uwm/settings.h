#ifndef UWM_SETTINGS_H
#define UWM_SETTINGS_H

#include <X11/xpm.h>

/* configuration datatypes */
/* ATTENTION: ONLY CHANGE TOGETHER WITH CONVERSION TABLE IN confparse.y */
#define UWM_S_INT 0
#define UWM_S_FLOAT 1
#define UWM_S_STRING 2
#define UWM_S_FONT 3

#define UWM_S_TYPENO 4

/* data index structure */
typedef struct _uwm_init_index {
  char *name;
  void *offset;
  int type;
} uwm_init_index; 

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

  void *ButtonsOnRoot;  /* to be implemented/changed */
  void *ButtonsOnWin;   /* to be implemented/changed */
} uwm_global_settings;

extern uwm_global_settings global_settings;

/* workspace specific settings */
typedef struct _uwm_workspace_settings {
/*** general workspace options */
  char *name;
  char *ScreenCommand;
  struct {
    Pixmap *image;
    XpmAttributes *attributes;
  } Wallpaper;

/*** colors */
  unsigned long ScreenColor;
  unsigned long InactiveColor, InactiveShadow, InactiveLight;
  unsigned long InactiveTitle;
  unsigned long ActiveColor, ActiveShadow, ActiveLight;
  unsigned long ActiveTitle;

  unsigned long BackgroundColor, BackgroundShadow, BackgroundLight;
  unsigned long ForegroundColor;
  unsigned long InactiveForeground;
  unsigned long InactiveBackground;
  unsigned long HighlightedForeground;
  unsigned long HighlightedBackground;
  unsigned long TextForeground;
  unsigned long TextBackground;
} uwm_workspace_settings;
#undef OFFSET_OF

#endif
