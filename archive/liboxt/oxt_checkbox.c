/*	oxt_checkbox.c -- check box widget
	vladimir ganchev */

#include "oxt_main.h"
#include "oxt_tree.h"
#include "oxt_internal.h"

void _checkbox_expose(struct oxt_Widget *Checkbox);
void _checkbox_buttonpress(struct oxt_Widget *Checkbox);
void _checkbox_buttonrelease(struct oxt_Widget *Checkbox);
void _checkbox_enter(struct oxt_Widget *Checkbox);
void _checkbox_leave(struct oxt_Widget *Checkbox);
void _checkbox_keypress(struct oxt_Widget *Checkbox);
void _checkbox_keyrelease(struct oxt_Widget *Checkbox);

oxt_Widget *oxt_CreateCheckbox(int x, int y, char *label, oxt_Widget *location) {
  oxt_Widget *Checkbox = malloc(sizeof(struct oxt_Widget));

  Checkbox->link = 0;
  Checkbox->buffer = malloc(1024);
  Checkbox->x = (x < 0) ? ((location->w)/2)-((XTextWidth(_defaultfont, label, strlen(label))+20)/2): x;
  Checkbox->y = (y < 0) ? ((location->h)/2)-7: y;
  Checkbox->w = XTextWidth(_defaultfont, label, strlen(label))+20;
  Checkbox->h = 15;
  Checkbox->buffer = label;
  Checkbox->parent = location->win;
  Checkbox->status = 0;
  Checkbox->id = CHECKBOX;
  Checkbox->pos = 0;

  Checkbox->win = XCreateSimpleWindow(_display, Checkbox->parent, Checkbox->x, Checkbox->y, Checkbox->w, Checkbox->h, 0, oxt_color.black, oxt_color.standard);

  Checkbox->gc = XCreateGC(_display, Checkbox->win, 0, 0);
  Checkbox->expose = (void *)_checkbox_expose;
  Checkbox->keypress = (void *)_checkbox_keypress;
  Checkbox->keyrelease = (void *)_checkbox_keyrelease;
  Checkbox->buttonpress = (void *)_checkbox_buttonpress;
  Checkbox->buttonrelease = (void *)_checkbox_buttonrelease;
  Checkbox->enter = (void *)_checkbox_enter;
  Checkbox->leave = (void *)_checkbox_leave;
  Checkbox->buttonmotion = 0;
  Checkbox->callback = 0;

  XSetFont(_display, Checkbox->gc, _defaultfont->fid);
  XSelectInput(_display, Checkbox->win, ExposureMask | EnterWindowMask | LeaveWindowMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask);

  if (_active == 0) {
    _active = Checkbox;
    Checkbox->active = 1;
  }

  _register_widget(Checkbox);
  return Checkbox;
}

void _checkbox_expose(struct oxt_Widget *Checkbox) {
  _frame(Checkbox, oxt_color.white, SUNKEN, 13, 13);
  XSetForeground(_display, Checkbox->gc, oxt_color.black);
  XDrawString(_display, Checkbox->win, Checkbox->gc, 20, 12, Checkbox->buffer, strlen(Checkbox->buffer));
  if (Checkbox->content)
    _drawcheck(Checkbox);
  if (Checkbox->active)
    _setstatus(Checkbox, ACTIVE);
}

void _checkbox_buttonpress(struct oxt_Widget *Checkbox) {
  if (_active != Checkbox) {
    if (_active->status == 1) return;
    _setstatus(_active, DEACTIVE);
    _active = Checkbox;
  }

  XSetForeground(_display, Checkbox->gc, oxt_color.standard);
  XFillRectangle(_display, Checkbox->win, Checkbox->gc, 2, 2, 9, 9);

  if (Checkbox->content)
    _drawcheck(Checkbox);

  _setstatus(Checkbox, ACTIVE);
  Checkbox->status = 1;
}

void _checkbox_buttonrelease(struct oxt_Widget *Checkbox) {
  if (_event.xbutton.x < Checkbox->w && _event.xbutton.x >= 0
	&& _event.xbutton.y < Checkbox->h && _event.xbutton.y >= 0 && Checkbox->status == 1) {
    XSetForeground(_display, Checkbox->gc, oxt_color.white);
    XFillRectangle(_display, Checkbox->win, Checkbox->gc, 2, 2, 9, 9);
    if (Checkbox->content)
      Checkbox->content = 0;
    else {
      _drawcheck(Checkbox);
      Checkbox->content = 1;
    }
  }

  Checkbox->status = 0;
}

void _checkbox_enter(struct oxt_Widget *Checkbox) {
  if (_rkey == 1) return;
  if (Checkbox->status == 1) Checkbox->buttonpress(Checkbox);
}

void _checkbox_leave(struct oxt_Widget *Checkbox) {
  if (_rkey == 1) return;
  if (Checkbox->status == 1) Checkbox->expose(Checkbox);
}

void _checkbox_keypress(struct oxt_Widget *Checkbox) {
  KeySym _key = XKeycodeToKeysym(_display, _event.xkey.keycode, 0);

  switch(_key) {
    case 32:
    case KEY_ENTER:
			_rkey = 1;
			Checkbox->buttonpress(Checkbox);
			break;
  }
}

void _checkbox_keyrelease(struct oxt_Widget *Checkbox) {
  KeySym _key = XKeycodeToKeysym(_display, _event.xkey.keycode, 0);

  switch(_key) {
    case 32:
    case KEY_ENTER:
			_rkey = 0;
			XSetForeground(_display, Checkbox->gc, oxt_color.white);
			XFillRectangle(_display, Checkbox->win, Checkbox->gc, 2, 2, 9, 9);

			if (Checkbox->content)
  			  Checkbox->content = 0;
 			else {
    			  _drawcheck(Checkbox);
   			  Checkbox->content = 1;
			}

			Checkbox->status = 0;
			break;
  }
}
