/*	oxt_edit.c -- editbox widget
	vladimir ganchev */

#include "oxt_main.h"
#include "oxt_tree.h"
#include "oxt_internal.h"

void _edit_expose(struct oxt_Widget *Edit);
void _edit_buttonpress(struct oxt_Widget *Edit);
void _edit_buttonrelease(struct oxt_Widget *Edit);
void _edit_buttonmotion(struct oxt_Widget *Edit);
void _edit_keypress(struct oxt_Widget *Edit);
void _edit_keyrelease(struct oxt_Widget *Edit);

oxt_Widget *oxt_CreateEdit(int x, int y, int w, char *buffer, oxt_Widget *location) {
  oxt_Widget *Edit = malloc(sizeof(struct oxt_Widget));
  Cursor _cursor;

  if (w == 0) {
    fprintf(stderr, "%s: window sized with zero!\n", _basename);
    exit(1);
  }

  Edit->x = x;
  Edit->y = y;
  Edit->w = w;
  Edit->h = 21;
  Edit->buffer = buffer;
  Edit->clipboard = malloc(strlen(Edit->buffer));
  Edit->parent = location->win;
  Edit->status = 0;
  Edit->id = EDIT;
  Edit->pos = 4;
  Edit->loc = 0;

  Edit->win  = XCreateSimpleWindow(_display, Edit->parent, x, y, w, 21, 0, oxt_color.standard, oxt_color.white);

  Edit->cb.sx = 0;
  Edit->cb.sx1 = 0;
  Edit->cb.sx2 = 0;
  Edit->cb.x = 0;
  Edit->cb.w = 0;
  Edit->gc = XCreateGC(_display, Edit->win, 0, 0);
  Edit->expose = (void *)_edit_expose;
  Edit->keypress = (void *)_edit_keypress;
  Edit->keyrelease = (void *)_edit_keyrelease;
  Edit->buttonpress = (void *)_edit_buttonpress;
  Edit->buttonrelease = (void *)_edit_buttonrelease;
  Edit->buttonmotion = (void *)_edit_buttonmotion;
  Edit->enter = 0;
  Edit->leave = 0;
  Edit->callback = 0;

  memset(Edit->clipboard, 0, strlen(Edit->clipboard));
  _cursor = XCreateFontCursor(_display, XC_xterm);
  XDefineCursor(_display, Edit->win, _cursor);
  XSetFont(_display, Edit->gc, _defaultfont->fid);
  XSelectInput(_display, Edit->win, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask);

  if (_active == 0) {
    _active = Edit;
    Edit->active = 1;
  }

  _register_widget(Edit);
  return Edit;
}

void _edit_expose(struct oxt_Widget *Edit) {
  _frame(Edit, 0, SUNKEN, -1, -1);
  XSetForeground(_display, Edit->gc, oxt_color.black);
  XDrawString(_display, Edit->win, Edit->gc, 4, 14, Edit->buffer, strlen(Edit->buffer));

  if (Edit->active)
    _setstatus(Edit, ACTIVE);
}

char *tostring(char ch) {  // okay, laugh!
  char *buff = malloc(1);
  buff[0] = ch;
  return buff;
}

void _edit_buttonmotion(struct oxt_Widget *Edit) {
  int i, count = 4;

  for (i=0; i<strlen(Edit->buffer); i++)
    if ((count <= _event.xmotion.x) &&
		(count+XTextWidth(_defaultfont, tostring(Edit->buffer[i]), 1) <= _event.xbutton.x))
      count += XTextWidth(_defaultfont, tostring(Edit->buffer[i]), 1);
    else break;

  _movecursor(Edit, Edit->pos, count);
  Edit->pos = count;
  Edit->loc = i;
}

void _edit_buttonrelease(struct oxt_Widget *Edit) {
  Edit->cb.sx = 0;
  Edit->cb.sx1 = 0;
  Edit->cb.sx2 = 0;
}

void _edit_buttonpress(struct oxt_Widget *Edit) {
  int tw = XTextWidth(_defaultfont, Edit->buffer, strlen(Edit->buffer)), i, count = 4;

  if (_active != Edit) {
    _setstatus(_active, DEACTIVE);
    _active = Edit;
    _setstatus(_active, ACTIVE);
  }

  if (_event.xbutton.x < tw+4) {
    for (i=0; i<strlen(Edit->buffer); i++)
      if ((count <= _event.xbutton.x) &&
		(count+XTextWidth(_defaultfont, tostring(Edit->buffer[i]), 1) <= _event.xbutton.x))
        count += XTextWidth(_defaultfont, tostring(Edit->buffer[i]), 1);
      else break;

    _movecursor(Edit, Edit->pos, count);
    Edit->pos = count;
    Edit->loc = i;

    XSetForeground(_display, Edit->gc, oxt_color.black);
    XDrawString(_display, Edit->win, Edit->gc, 4, 14, Edit->buffer, strlen(Edit->buffer));

  } else {
    _movecursor(Edit, Edit->pos, tw+4);
    Edit->pos = tw+4;
    Edit->loc = strlen(Edit->buffer);
  }
}

void _edit_keypress(struct oxt_Widget *Edit) {
  KeySym _key = XKeycodeToKeysym(_display, _event.xkey.keycode, 0);
  char buffer[100], temp1[1024], temp2[1024];
  int oldpos = Edit->pos, junk, temp;

  memset(buffer, 0, strlen(buffer));
  buffer[0] = _key;

  switch(_key) {
    case KEY_ENTER:
			break;
    case KEY_LEFTARROW:
			if (Edit->loc == 0 || Edit->pos <= 4) return;
			sprintf(buffer, "%c", Edit->buffer[Edit->loc-1]);
			Edit->pos -= XTextWidth(_defaultfont, buffer, 1);
			Edit->loc--;
			_movecursor(Edit, oldpos, Edit->pos);
			XSetForeground(_display, Edit->gc, oxt_color.black);
			XDrawString(_display, Edit->win, Edit->gc, 4, 14, Edit->buffer, strlen(Edit->buffer));
			break;
    case KEY_RIGHTARROW:
			if (Edit->loc == strlen(Edit->buffer)) return;
			sprintf(buffer, "%c", Edit->buffer[Edit->loc]);
			Edit->pos += XTextWidth(_defaultfont, buffer, strlen(buffer));
			Edit->loc++;
			_movecursor(Edit, oldpos, Edit->pos);
                        XSetForeground(_display, Edit->gc, oxt_color.black);
                        XDrawString(_display, Edit->win, Edit->gc, 4, 14, Edit->buffer, strlen(Edit->buffer));
			break;
    case KEY_HOME:
			Edit->loc = 0;
			_movecursor(Edit, Edit->pos, 4);
			Edit->pos = 4;
			break;
    case KEY_END:
			Edit->loc = strlen(Edit->buffer);
			_movecursor(Edit, Edit->pos, XTextWidth(_defaultfont, Edit->buffer, strlen(Edit->buffer))+4);
			Edit->pos = XTextWidth(_defaultfont, Edit->buffer, strlen(Edit->buffer))+4;
			break;
    case KEY_LEFTSHIFT:
    case KEY_RIGHTSHIFT:
			Edit->shift = 1;
			break;
    case KEY_CAPS:	Edit->caps = (Edit->caps) ? 0:1;
			break;
    case KEY_DELETE:
    case KEY_BACKSPACE:
			if (Edit->pos <= 4) break;
			if (Edit->loc < strlen(Edit->buffer)) {
			  junk = Edit->buffer[Edit->loc-1];
			  memset(temp1, 0, strlen(temp1));
			  memset(temp2, 0, strlen(temp2));
			  strncat(temp1, Edit->buffer, Edit->loc-1);
			  sprintf(temp2, "%s%s", temp1, Edit->buffer+Edit->loc);
			  memset(Edit->buffer, 0, strlen(Edit->buffer));
			  strcat(Edit->buffer, temp2);

			  XSetForeground(_display, Edit->gc, oxt_color.white);
			  XFillRectangle(_display, Edit->win, Edit->gc, 2, 2, Edit->w-4, Edit->h-4);
			  XSetForeground(_display, Edit->gc, oxt_color.black);
			  XDrawString(_display, Edit->win, Edit->gc, 4, 14, Edit->buffer, strlen(Edit->buffer));

			  sprintf(buffer, "%c", junk);
			  temp = (junk == 32) ? XTextWidth(_defaultfont, " ", 1) : XTextWidth(_defaultfont, buffer, 1);
			  Edit->pos -= temp;
			  _movecursor(Edit, 0, Edit->pos);
			  Edit->loc--;
			} else {
			  sprintf(buffer, "%c", Edit->buffer[Edit->loc-1]);
			  Edit->buffer[Edit->loc-1] = 0;
			  temp = (_key == 32) ? XTextWidth(_defaultfont, " ", 1) : XTextWidth(_defaultfont, buffer, 1);
			  XSetForeground(_display, Edit->gc, oxt_color.white);
			  XFillRectangle(_display, Edit->win, Edit->gc, (Edit->pos)-temp, 4, temp, 14);
			  Edit->pos -= temp;
			  _movecursor(Edit, Edit->pos+temp, Edit->pos);
			  Edit->loc--;
			}
			break;
    case KEY_LEFTALT:
    case KEY_RIGHTALT:
    case KEY_LEFTCTRL:
    case KEY_RIGHTCTRL: break;
    case 0: break;

    default:
			if (Edit->shift) {
			  switch(_key) {
			    case '0': _key = ')'; break;
			    case '1': _key = '!'; break;
			    case '2': _key = '@'; break;
			    case '3': _key = '#'; break;
			    case '4': _key = '$'; break;
			    case '5': _key = '%'; break;
			    case '6': _key = '^'; break;
			    case '7': _key = '&'; break;
			    case '8': _key = '*'; break;
			    case '9': _key = '('; break;
			    case '-': _key = '_'; break;
			    case '=': _key = '+'; break;
			    case '\\': _key = '|'; break;
			    case '[': _key = '{'; break;
			    case ']': _key = '}'; break;
			    case ';': _key = ':'; break;
			    case '\'': _key = '"'; break;
			    case '/': _key = '?'; break;
			    case '.': _key = '>'; break;
			    case ',': _key = '<'; break;
			    case '`': _key = '~'; break;
			    case ' ': _key = ' '; break;
			    default: _key -= 32; break;
			  }
			} else if (Edit->caps) if (_key >= 97 && _key <= 122) _key -= 32;

			if (_key == 159 || Edit->pos >= (Edit->w)-12) break;
			memset(buffer, 0, strlen(buffer));
			buffer[0] = _key;
			temp = XTextWidth(_defaultfont, buffer, 1);
			Edit->pos += temp;
			Edit->loc++;

			if (((Edit->loc)-1) < strlen(Edit->buffer)) {
			  memset(temp1, 0, strlen(temp1));
			  memset(temp2, 0, strlen(temp2));
			  strncat(temp1, Edit->buffer, Edit->loc-1);
                          sprintf(temp2, "%s%s%s", temp1, buffer, (Edit->buffer)+(Edit->loc)-1);
                          memset(Edit->buffer, 0, strlen(Edit->buffer));
                          strcat(Edit->buffer, temp2);
                          XSetForeground(_display, Edit->gc, oxt_color.white);
                          XFillRectangle(_display, Edit->win, Edit->gc, 2, 2, (Edit->w)-4, (Edit->h)-4);
                        } else strcat(Edit->buffer, buffer);

                        XSetForeground(_display, Edit->gc, oxt_color.black);
                        _movecursor(Edit, (Edit->pos)-temp, Edit->pos);
                        XDrawString(_display, Edit->win, Edit->gc, 4, 14, Edit->buffer, strlen(Edit->buffer));

  }
}

void _edit_keyrelease(struct oxt_Widget *Edit) {
  KeySym _key = XKeycodeToKeysym(_display, _event.xkey.keycode, 0);

  if ((_key == KEY_LEFTSHIFT) || (_key == KEY_RIGHTSHIFT)) Edit->shift = 0;
}
