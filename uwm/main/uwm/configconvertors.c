/* data conversion routines for the ude config file parser */
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

#include <X11/Xlib.h>

#include "settings.h"
#include "confparse.h"
#include "uwm.h"
#include "special.h"
#include "init.h"
#include "pix.h"

#define derefptr(TYPE) ((TYPE *)(((void *)base) + (out->offset)))
#define deref(TYPE) (*(derefptr(TYPE)))

extern Display *disp;
extern UDEScreen TheScreen;

char *uopt_int_int(YYSTYPE *in, const uwm_init_index *out, void *base)
{
  deref(int) = in->intval;
  return NULL;
}

char *uopt_int_flt(YYSTYPE *in, const uwm_init_index *out, void *base)
{
  double d;
  d = (double)in->intval;
  deref(double) = d;
  return NULL;
}

char *uopt_flt_flt(YYSTYPE *in, const uwm_init_index *out, void *base)
{
  deref(double) = in->floatval;
  return NULL;
}

char *uopt_str_int(YYSTYPE *in, const uwm_init_index *out, void *base)
{
  deref(int) = atoi(in->string);
  free(in->string);
  return NULL;
}

char *uopt_str_flt(YYSTYPE *in, const uwm_init_index *out, void *base)
{
  deref(double) = atof(in->string);
  free(in->string);
  return NULL;
}

char *uopt_str_str(YYSTYPE *in, const uwm_init_index *out, void *base)
{
  deref(char *) = in->string;
  return NULL;
}

char *uopt_str_fnt(YYSTYPE *in, const uwm_init_index *out, void *base)
{
  XFontStruct *nxfs;
  if(nxfs = XLoadQueryFont(disp, in->string)) {
    FontStruct *oldfs, *fs;
    if(oldfs = deref(FontStruct*)) {
      if(oldfs->xfs) XFreeFont(disp, oldfs->xfs);
      if(oldfs->name) free(oldfs->name);
      free(oldfs);
    }
    fs = MyCalloc(1, sizeof(FontStruct));
    fs->xfs = nxfs;
    fs->name = in->string;
    deref(FontStruct*) = fs;
  } else {
    static char errmsg[256] = "Could not load font \"";
    sprintf(errmsg + 21, "%.232s\"\n", in->string);
    free(in->string);
    return(errmsg);
  }
  return(NULL);
}

char *MultiplyColor(char *color, double factor)
{
  XColor xcol;
  unsigned int r, g, b;
  char *ret;
  ret = MyCalloc(sizeof(char), 14);

  if(XParseColor(disp, TheScreen.colormap, color, &xcol)) {
    r = xcol.red * factor; r = (r > 0xFFFF) ? 0xFFFF : r;
    g = xcol.green * factor; g = (g > 0xFFFF) ? 0xFFFF : g;
    b = xcol.blue * factor; b = (b > 0xFFFF) ? 0xFFFF : b;
  } else {
    if(factor > 1) {
      r = g = b = 0xFFFF;
    } else {
      r = g = b = 0x0000;
    }
  }
  sprintf(ret, "#%.4X%.4X%.4X", r, g, b);
  return(ret);
}

char *uopt_str_col(YYSTYPE *in, const uwm_init_index *out, void *base)
{
  XColor *xcol;
  xcol = MyCalloc(1, sizeof(*xcol));
  if(XParseColor(disp, TheScreen.colormap, in->string, xcol)) {
    if(deref(XColor*)) {
      XColor *oldcol = deref(XColor*);
      FreeColor(oldcol);
      free(oldcol);
    }
    AllocColor(xcol);
    deref(XColor*) = xcol;
    free(in->string);
  } else {
    static char errmsg[128] = "Could not load color \"";
    sprintf(errmsg + 22, "%.103s\"\n", in->string);
    free(xcol);
    free(in->string);
    return(errmsg);
  }
  return(NULL);
}

char *uopt_str_pix(YYSTYPE *in, const uwm_init_index *out, void *base)
{
  uwm_image *img;
  img = MyCalloc(1, sizeof(uwm_image));
  img->attributes = MyCalloc(1, sizeof(XpmAttributes));

  if(LoadPic(in->string, &(img->image), img->attributes)) {
    if(deref(uwm_image*)) {
      FreePic(&(deref(uwm_image*)->image),
              &(deref(uwm_image*)->attributes));
      free(deref(uwm_image*));
    }
    deref(uwm_image*) = img;
    free(in->string);
  } else {
    static char errmsg[256] = "Could not load background image \"";
    sprintf(errmsg + 33, "%.220s\"\n", in->string);
    free(img->attributes);
    free(img);
    free(in->string);
    return(errmsg);
  }
  return(NULL);
}
