/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifdef CF__PROTOTYPES
#undef CF__PROTOTYPES
#define CONFIG_FUNC_COUNT 20
#define cf_function(NAME, RETTYPE) cf_data *cf_##NAME(cf_data_array *args);
#elif defined CF__STRUCT
#undef CF__STRUCT
#define cf_function(NAME, RETTYPE) {RETTYPE, cf_##NAME, #NAME},
#else
#error EITHER CF__PROTOTYPES OR CF__STRUCT MUST BE DEFINED WHEN INCLUDING conf_func_list.h
#endif
#ifdef cf_function
cf_function( Add,		CFD_INT)	/* (int a, int b) */
cf_function( GetMouseX,		CFD_INT)	/* () */
cf_function( GetMouseY,		CFD_INT)	/* () */
cf_function( MoveMouse,		CFD_VOID)	/* (int x, int y) */
cf_function( GetWS,		CFD_INT)	/* () */
cf_function( SetWS,		CFD_INT)	/* (int WS) */
cf_function( NextWS,		CFD_INT)	/* () */
cf_function( PrevWS,		CFD_INT)	/* () */
cf_function( NextWin,		CFD_VOID)	/* () */
cf_function( PrevWin,		CFD_VOID)	/* () */
cf_function( GetWinX,		CFD_INT)	/* () */
cf_function( GetWinY,		CFD_INT)	/* () */
cf_function( GetWinWidth,	CFD_INT)	/* () */
cf_function( GetWinHeight,	CFD_INT)	/* () */
cf_function( MoveWin,		CFD_VOID)	/* (int x, int y) */
cf_function( ResizeWin,		CFD_VOID)	/* (int w, int h) */
cf_function( RaiseWin,		CFD_VOID)	/* () */
cf_function( LowerWin,		CFD_VOID)	/* () */
cf_function( WinToWS,		CFD_VOID)	/* (int WS) */
cf_function( StickyWin,		CFD_VOID)	/* () */
#undef cf_function
#endif /* cf_function */
