#ifndef XTEXTBOX_H_
#define XTEXTBOX_H_

#include "xobject.h"

XObject* x_textbox_create(XObject* parent, int x, int y, unsigned width, unsigned height, Bool visible,
		XPointer cbdata);
void x_textbox_exposed(XObject* t);
void x_textbox_insert_text(XObject* t,int pos,char* str, int len);
void x_textbox_erase_text(XObject* t, int l, int r);
void x_textbox_move_cursor_left(XObject* t,Bool Shift);
void x_textbox_move_cursor_right(XObject* t,Bool Shift);
void x_textbox_copy_selected(XObject* t);
void x_textbox_paste(XObject* t,int pos);
void x_textbox_set_visible(XObject* t, Bool visible);
void x_textbox_set_text(XObject *t, char* text);
void x_textbox_set_change(XObject* t, CallbackLite f);
void x_textbox_handle_event(XObject* obj,XEvent event);
void x_textbox_update_offset(XObject* t);
#endif

