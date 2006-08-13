/* settings_workspace_list.h: defines workspace specific configuration
   options */

/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifdef WO__STRUCT
#define UWM_WORKSPACE_OPTION_NR 21
#undef WO__STRUCT

#define TY_INT int
#define TY_STRING char *
#define TY_FONT FontStruct *
#define TY_FLOAT double
#define TY_PIXMAP uwm_image *
#define TY_COLOR XColor *
#define WORKSPACE_OPTION(NAME, TYPE, DEFAULT) \
	TYPE NAME;

#elif defined WO__DEFINITIONS
#undef WO__DEFINITIONS

#define TY_INT UWM_S_INT
#define TY_STRING UWM_S_STRING
#define TY_FONT UWM_S_FONT
#define TY_FLOAT UWM_S_FLOAT
#define TY_PIXMAP UWM_S_PIXMAP
#define TY_COLOR UWM_S_COLOR
#define WORKSPACE_OPTION(NAME, TYPE, DEFAULT) \
        {#NAME,  OFFSET_OF(_uwm_workspace_settings, NAME),  TYPE,  DEFAULT},

#else
#error EITHER WO__STRUCT OR WO__DEFINITIONS MUST BE DEFINED WHEN INCLUDING settings_workspace_list.h
#endif
#ifdef WORKSPACE_OPTION
  WORKSPACE_OPTION(Name, TY_STRING, uwm_default_ws_name)
  WORKSPACE_OPTION(Wallpaper, TY_PIXMAP, NULL)
  WORKSPACE_OPTION(ScreenColor, TY_COLOR, "grey30")
  WORKSPACE_OPTION(InactiveColor, TY_COLOR, "grey30")
  WORKSPACE_OPTION(InactiveShadow, TY_COLOR, "grey10")
  WORKSPACE_OPTION(InactiveLight, TY_COLOR, "grey50")
  WORKSPACE_OPTION(InactiveTitle, TY_COLOR, "white")
  WORKSPACE_OPTION(ActiveColor, TY_COLOR, "grey70")
  WORKSPACE_OPTION(ActiveShadow, TY_COLOR, "grey50")
  WORKSPACE_OPTION(ActiveLight, TY_COLOR, "grey90")
  WORKSPACE_OPTION(ActiveTitle, TY_COLOR, "black")
  WORKSPACE_OPTION(BackgroundColor, TY_COLOR, "grey30")
  WORKSPACE_OPTION(BackgroundShadow, TY_COLOR, "grey10")
  WORKSPACE_OPTION(BackgroundLight, TY_COLOR, "grey50")
  WORKSPACE_OPTION(ForegroundColor, TY_COLOR, "white")
  WORKSPACE_OPTION(InactiveForeground, TY_COLOR, "grey70")
  WORKSPACE_OPTION(InactiveBackground, TY_COLOR, "grey20")
  WORKSPACE_OPTION(HighlightedForeground, TY_COLOR, "white")
  WORKSPACE_OPTION(HighlightedBackground, TY_COLOR, "grey50")
  WORKSPACE_OPTION(TextForeground, TY_COLOR, "white")
  WORKSPACE_OPTION(TextBackground, TY_COLOR, "black")
#undef WORKSPACE_OPTION
#endif /* WORKSPACE_OPTION */
#undef TY_INT
#undef TY_STRING
#undef TY_FONT
#undef TY_FLOAT
#undef TY_PIXMAP
#undef TY_COLOR
