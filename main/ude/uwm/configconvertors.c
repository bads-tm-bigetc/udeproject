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

#include "settings.h"
#include "confparse.h"

#define deref(TYPE) (*((TYPE *)((base) + ((unsigned long)out->offset))))

void uopt_int_int(YYSTYPE *in, uwm_init_index *out, void *base)
{
  deref(int) = in->intval;
}

void uopt_int_flt(YYSTYPE *in, uwm_init_index *out, void *base)
{
  double d;
  d = (double)in->intval;
  deref(double) = d;
}

void uopt_flt_flt(YYSTYPE *in, uwm_init_index *out, void *base)
{
  deref(double) = in->floatval;
}

void uopt_str_str(YYSTYPE *in, uwm_init_index *out, void *base)
{
  deref(char *) = in->string;
}
