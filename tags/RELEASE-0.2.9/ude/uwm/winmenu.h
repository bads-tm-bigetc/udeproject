#ifndef UWM_WINMENU_H
#define UWM_WINMENU_H

void StartWinMenu(UltimateContext *uc,int x,int y);
void WinMenuEnterNotify(XEvent *event);
void WinMenuButtonRelease(XEvent *event);
void WinMenuButtonPress(XEvent *event);
void WinMenuVisibility(XEvent *event);
void WinMenuUnmapNotify(XEvent *event);

#endif
