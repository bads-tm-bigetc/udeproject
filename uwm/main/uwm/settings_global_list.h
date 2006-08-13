/* settings_global_list.h: defines global configuration options */

/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifdef GO__STRUCT
#define UWM_GLOBAL_OPTION_NR 27
#undef GO__STRUCT

#define TY_INT int
#define TY_STRING char *
#define TY_FONT FontStruct *
#define TY_FLOAT double
#define GLOBAL_OPTION(NAME, TYPE, DEFAULT) \
	TYPE NAME;

#elif defined GO__DEFINITIONS
#undef GO__DEFINITIONS

#define TY_INT UWM_S_INT
#define TY_STRING UWM_S_STRING
#define TY_FONT UWM_S_FONT
#define TY_FLOAT UWM_S_FLOAT
#define GLOBAL_OPTION(NAME, TYPE, DEFAULT) \
	{#NAME,  OFFSET_OF(_uwm_global_settings, NAME),  TYPE,  DEFAULT},

#else
#error EITHER GO__STRUCT OR GO__DEFINITIONS MUST BE DEFINED WHEN INCLUDING settings_global_list.h
#endif

#ifdef GLOBAL_OPTION
/*** geometry */
  GLOBAL_OPTION(BorderWidth, TY_INT, "10")
  GLOBAL_OPTION(TransientBorderWidth, TY_INT, "3")
  GLOBAL_OPTION(TitleHeight, TY_INT, "0")
  GLOBAL_OPTION(FrameBevelWidth, TY_INT, "2")
  GLOBAL_OPTION(FrameFlags, TY_INT, "39")
  GLOBAL_OPTION(HexPath, TY_STRING, UDE_DIR "/gfx")

/*** general layout */
  GLOBAL_OPTION(LayoutFlags, TY_INT, "0")
  GLOBAL_OPTION(BevelWidth, TY_INT, "2")
  GLOBAL_OPTION(MenuXOffset, TY_INT, "2")
  GLOBAL_OPTION(MenuYOffset, TY_INT, "2")

/*** fonts */
  GLOBAL_OPTION(TitleFont, TY_FONT, "fixed")
  GLOBAL_OPTION(Font, TY_FONT, "fixed")
  GLOBAL_OPTION(MonoFont, TY_FONT, "fixed")
  GLOBAL_OPTION(HighlightFont, TY_FONT, "fixed")
  GLOBAL_OPTION(InactiveFont, TY_FONT, "fixed")

/*** batch */
  GLOBAL_OPTION(StartScript, TY_STRING, NULL)
  GLOBAL_OPTION(StopScript, TY_STRING, NULL)
  GLOBAL_OPTION(ResourceFile, TY_STRING, NULL)

/*** behaviour */
  GLOBAL_OPTION(PlacementStrategy, TY_INT, "5")
  GLOBAL_OPTION(PlacementThreshold, TY_INT, "0")
  GLOBAL_OPTION(OpaqueMoveSize, TY_INT, "0")

  GLOBAL_OPTION(MaxWinWidth, TY_INT, "0")
  GLOBAL_OPTION(MaxWinHeight, TY_INT, "0")

  GLOBAL_OPTION(WarpPointerToNewWinH, TY_FLOAT, "-1")
  GLOBAL_OPTION(WarpPointerToNewWinV, TY_FLOAT, "-1")

  GLOBAL_OPTION(SnapDistance, TY_INT, "10")
  GLOBAL_OPTION(BehaviourFlags, TY_INT, "0")
#undef GLOBAL_OPTION
#endif /* GLOBAL_OPTION */
#undef TY_INT
#undef TY_STRING
#undef TY_FONT
#undef TY_FLOAT
