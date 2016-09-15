/*	oxt_button.c -- button widget
	vladimir ganchev */

#include "oxt_main.h"
#include "oxt_tree.h"
#include "oxt_internal.h"

void _button_expose(struct oxt_Widget *Button);
void _button_buttonpress(struct oxt_Widget *Button);
void _button_buttonrelease(struct oxt_Widget *Button);
void _button_enter(struct oxt_Widget *Button);
void _button_leave(struct oxt_Widget *Button);
void _button_keypress(struct oxt_Widget *Button);
void _button_keyrelease(struct oxt_Widget *Button);

oxt_Widget *oxt_CreateButton(int x, int y, int w, int h, char *label, oxt_Widget *location) {
  oxt_Widget *Button = malloc(sizeof(struct oxt_Widget));

  if (w == 0 || h == 0) {
    fprintf(stderr, "%s: window sized with zero!\n", _basename);
    exit(1);
  }

  Button->link = 0;
  Button->buffer = malloc(1024);
  Button->x = (x < 0) ? ((location->w)/2)-(w/2): x;
  Button->y = (y < 0) ? ((location->h)/2)-(h/2): y;
  Button->w = w;
  Button->h = h;
  Button->buffer = label;
  Button->parent = location->win;
  Button->status = 0;
  Button->id = BUTTON;

  Button->win = XCreateSimpleWindow(_display, Button->parent, Button->x, Button->y, w, h, 0, oxt_color.black, oxt_color.standard);

  Button->gc = XCreateGC(_display, Button->win, 0, 0);
  Button->expose = (void *)_button_expose;
  Button->keypress = (void *)_button_keypress;
  Button->keyrelease = (void *)_button_keyrelease;
  Button->buttonpress = (void *)_button_buttonpress;
  Button->buttonrelease = (void *)_button_buttonrelease;
  Button->enter = (void *)_button_enter;
  Button->leave = (void *)_button_leave;
  Button->buttonmotion = 0;
  Button->callback = 0;

  XSetFont(_display, Button->gc, _defaultfont->fid);
  XSelectInput(_display, Button->win, StructureNotifyMask | ExposureMask | EnterWindowMask | LeaveWindowMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask);

  if (_active == 0) {    /* if this is the first created */
    _active = Button;    /* widget, start the tree with it */
    Button->active = 1;
  }

  _register_widget(Button);
  return Button;
}

void _button_expose(struct oxt_Widget *Button) {
  int twidth = XTextWidth(_defaultfont, Button->buffer, strlen(Button->buffer));

  if (twidth > (Button->w)-6) {   /* check if the text in the button won't fit */
    XResizeWindow(_display, Button->win, (Button->w)+(twidth-((Button->w)-6)), Button->h);
    Button->w = (Button->w)+(twidth-((Button->w)-6));
  }

  _frame(Button, oxt_color.standard, RAISED, -1, -1);
  XSetForeground(_display, Button->gc, oxt_color.black);
  XDrawString(_display, Button->win, Button->gc, ((Button->w)/2)-(twidth)/2,
								((Button->h)/2)+4, Button->buffer, strlen(Button->buffer));
  if (Button->active)
    _setstatus(Button, ACTIVE);
}

void _button_buttonpress(struct oxt_Widget *Button) {
  if (_active != Button) {   /* we have to set this widget active */
    if (_active->status == 1) return;
    _setstatus(_active, DEACTIVE);
    _active = Button;
  }

  _frame(Button, oxt_GetColor(0xaaaa, 0xaaaa, 0xaaaa), SUNKEN, -1, -1);
  XSetForeground(_display, Button->gc, oxt_color.black);
  XDrawString(_display, Button->win, Button->gc, (((Button->w)/2)-(XTextWidth(_defaultfont, Button->buffer,
											strlen(Button->buffer))/2))+1,
								((Button->h)/2)+5, Button->buffer, strlen(Button->buffer));
  _setstatus(Button, ACTIVE);

  Button->status = 1;
}

void _button_buttonrelease(struct oxt_Widget *Button) {
  /* make sure the release was *ON* the button, then the callback */
  if (_event.xbutton.x < Button->w && _event.xbutton.x >= 0
	&& _event.xbutton.y < Button->h && _event.xbutton.y >= 0 && Button->status == 1) {
    if (Button->callback != 0)
      Button->callback(Button);  /* now the waited callback */
    _frame(Button, oxt_color.standard, RAISED, -1, -1);
    XSetForeground(_display, Button->gc, oxt_color.black);
    XDrawString(_display, Button->win, Button->gc, ((Button->w)/2)-(XTextWidth(_defaultfont, Button->buffer,
			strlen(Button->buffer)))/2, ((Button->h)/2)+4, Button->buffer, strlen(Button->buffer));
    if (Button->active)
      XDrawRectangle(_display, Button->win, Button->gc, 2, 2, (Button->w)-5, (Button->h)-5);
  }

  Button->status = 0;
}

void _button_enter(struct oxt_Widget *Button) {
  if (_rkey == 1) return;
  if (Button->status == 1) Button->buttonpress(Button); /* ok, so this is somehow strange.
							we call the buttonpress function
							when we enter the button but only
							if the button was pressed before.
							this is because we redraw the normal
							button on Leave event when the
							button was still pressed. */
}

void _button_leave(struct oxt_Widget *Button) {
  if (_rkey == 1) return;
  if (Button->status == 1) Button->expose(Button);  /* redraw the normal state button on Leave
							event only if the button was pressed */
}

void _button_keypress(struct oxt_Widget *Button) {
  KeySym _bkey = XKeycodeToKeysym(_display, _event.xkey.keycode, 0);

  switch(_bkey) {
    case 32:
    case KEY_ENTER:
			_rkey = 1;
			Button->buttonpress(Button);
			break;
  }
}

void _button_keyrelease(struct oxt_Widget *Button) {
  KeySym _bkey = XKeycodeToKeysym(_display, _event.xkey.keycode, 0);

  switch(_bkey) {
    case 32:
    case KEY_ENTER:
			_rkey = 0;
			if (Button->callback != 0) Button->callback(Button);
			Button->expose(Button);
			Button->status = 0;
			break;
  }
}

