/***  CONF_FUNC.C: contains configuration file functions  ***/

/* ########################################################################

   uwm - THE ude WINDOW MANAGER

   ########################################################################

   Copyright (c) : Christian Ruppert

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   ######################################################################## */

#include "conf_func.h"

const cf_func_def cf_functions[CONFIG_FUNC_COUNT] = {
#define CF__STRUCT
#include "conf_func_list.h"
};

cf_data *cf_Add(cf_data_array *args) {
}

cf_data *cf_GetMouseX(cf_data_array *args) {
}

cf_data *cf_GetMouseY(cf_data_array *args) {
}

cf_data *cf_MoveMouse(cf_data_array *args) {
}

cf_data *cf_GetWS(cf_data_array *args) {
}

cf_data *cf_SetWS(cf_data_array *args) {
}

cf_data *cf_NextWS(cf_data_array *args) {
}

cf_data *cf_PrevWS(cf_data_array *args) {
}

cf_data *cf_NextWin(cf_data_array *args) {
}

cf_data *cf_PrevWin(cf_data_array *args) {
}

cf_data *cf_GetWinX(cf_data_array *args) {
}

cf_data *cf_GetWinY(cf_data_array *args) {
}

cf_data *cf_GetWinWidth(cf_data_array *args) {
}

cf_data *cf_GetWinHeight(cf_data_array *args) {
}

cf_data *cf_MoveWin(cf_data_array *args) {
}

cf_data *cf_ResizeWin(cf_data_array *args) {
}

cf_data *cf_RaiseWin(cf_data_array *args) {
}

cf_data *cf_LowerWin(cf_data_array *args) {
}

cf_data *cf_WinToWS(cf_data_array *args) {
}

cf_data *cf_StickyWin(cf_data_array *args) {
}

