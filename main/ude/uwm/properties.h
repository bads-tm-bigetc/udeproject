/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifndef UWM_PROPERTIES_H
#define UWM_PROPERTIES_H

void UpdateUWMContext(UltimateContext *uc);
void UpdateName(UltimateContext *uc);
void UpdateIconName(UltimateContext *uc);
void Updatera(UltimateContext *uc);
void UpdateWMHints(UltimateContext *uc);
void UpdateMotifHints(UltimateContext *uc);
void UpdateTransientForHint(UltimateContext *uc);
void UpdateWMProtocols(UltimateContext *uc);
void SetIsMapState(UltimateContext *uc,int state);
void SetSeemsMapState(UltimateContext *uc,int state);

#endif
