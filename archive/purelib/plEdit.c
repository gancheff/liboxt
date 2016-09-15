#include "purelib.h"

void plEditExpose(struct plWidget_t *Edit);
void plEditButtonPress(struct plWidget_t *Edit);
void plEditKeyPress(struct plWidget_t *Edit);

plWidget *plEditCreate(int x, int y, int w, char *buffer, plWidget *location) {
  plWidget *Edit = malloc(sizeof(struct plWidget_t));
  Cursor _plCursor;

  if (w == 0) {
    fprintf(stderr, "[error] %s: window sized with zero!\n", _plName);
    exit(1);
  }

  Edit->x = x;
  Edit->y = y;
  Edit->w = w;
  Edit->h = 21;
  Edit->buffer = buffer;
  Edit->parent = location->win;
  Edit->status = 0;
  Edit->id = EDIT;
  Edit->pos = 4;
  Edit->loc = 0;

  Edit->win = XCreateSimpleWindow(_plDisplay, Edit->parent, x, y, w, 21, 0, plStandardColor, plColorWhite);

  Edit->gc = XCreateGC(_plDisplay, Edit->win, 0, 0);
  Edit->plExpose = (void *)plEditExpose;
  Edit->plKeyPress = (void *)plEditKeyPress;
  Edit->plKeyRelease = 0;
  Edit->plButtonPress = (void *)plEditButtonPress;
  Edit->plButtonRelease = 0;
  Edit->plCallback = 0;

  _plCursor = XCreateFontCursor(_plDisplay, XC_xterm);
  XDefineCursor(_plDisplay, Edit->win, _plCursor);
  XSetFont(_plDisplay, Edit->gc, _plDefaultFont->fid);
  XSelectInput(_plDisplay, Edit->win, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask);

  _plStack[_plStackCount] = Edit;
  _plStackCount++;

  XMapWindow(_plDisplay, Edit->win);

#ifdef DEBUG
  printf("[debug] window #%d created\n", _plStackCount);
#endif
  return Edit;
}

void plEditExpose(struct plWidget_t *Edit) {
  _plFrame(Edit, 0, SUNKEN);
  XSetForeground(_plDisplay, Edit->gc, plColorBlack);
  XDrawString(_plDisplay, Edit->win, Edit->gc, 4, 14, Edit->buffer, strlen(Edit->buffer));
  if (Edit->active)
    _plSetStatus(Edit, ACTIVE);
}

void plEditButtonPress(struct plWidget_t *Edit) {
  if (_plCurrent != Edit) {
    _plCurrent->active = 0;
    _plSetStatus(_plCurrent, DEACTIVE);
    _plCurrent = Edit;
    Edit->active = 1;
    _plSetStatus(_plCurrent, ACTIVE);
  }
}

void plEditKeyPress(struct plWidget_t *Edit) {
  KeySym plKey = XKeycodeToKeysym(_plDisplay, _plEvent.xkey.keycode, 0);
  char buffer[100], temp1[1024], temp2[1024];
  int oldpos = Edit->pos, junk, temp;

  sprintf(buffer, "%c", plKey);

  switch(plKey) {
    case KEY_LEFTARROW:
			if (Edit->loc == 0) return;
			Edit->pos -= XTextWidth(_plDefaultFont, buffer, strlen(buffer));
			Edit->loc--;
			_plMoveCursor(Edit, oldpos, Edit->pos);
			break;
    case KEY_RIGHTARROW:
			if (Edit->loc == strlen(Edit->buffer)) return;
			Edit->pos += XTextWidth(_plDefaultFont, buffer, strlen(buffer));
			Edit->loc++;
			_plMoveCursor(Edit, oldpos, Edit->pos);
			break;
    case KEY_HOME:
			Edit->loc = 0;
			_plMoveCursor(Edit, Edit->pos, 4);
			Edit->pos = 4;
			break;
    case KEY_END:
			Edit->loc = strlen(Edit->buffer);
			_plMoveCursor(Edit, Edit->pos, XTextWidth(_plDefaultFont, Edit->buffer, strlen(Edit->buffer))+4);
			Edit->pos = XTextWidth(_plDefaultFont, Edit->buffer, strlen(Edit->buffer))+4;
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

			  XSetForeground(_plDisplay, Edit->gc, plColorWhite);
			  XFillRectangle(_plDisplay, Edit->win, Edit->gc, 2, 2, Edit->w-4, Edit->h-4);
			  XSetForeground(_plDisplay, Edit->gc, plColorBlack);
			  XDrawString(_plDisplay, Edit->win, Edit->gc, 4, 14, Edit->buffer, strlen(Edit->buffer));
			  sprintf(buffer, "%c", junk);
			  temp = (junk == 32) ? XTextWidth(_plDefaultFont, " ", 1) : XTextWidth(_plDefaultFont, buffer, 1);
			  Edit->pos -= temp;
			  _plMoveCursor(Edit, 0, Edit->pos);
			  Edit->loc--;
			} else {
			  sprintf(buffer, "%c", Edit->buffer[Edit->loc-1]);
			  Edit->buffer[Edit->loc-1] = 0;
			  temp = (plKey == 32) ? XTextWidth(_plDefaultFont, " ", 1) : XTextWidth(_plDefaultFont, buffer, 1);
			  XSetForeground(_plDisplay, Edit->gc, plColorWhite);
			  XFillRectangle(_plDisplay, Edit->win, Edit->gc, (Edit->pos)-temp, 4, temp, 14);
			  Edit->pos -= temp;
			  _plMoveCursor(Edit, Edit->pos+temp, Edit->pos);
			  Edit->loc--;
			}
			break;
    default:
  			XSetForeground(_plDisplay, Edit->gc, plColorBlack);
  			XDrawString(_plDisplay, Edit->win, Edit->gc, Edit->pos, 14, buffer, strlen(buffer));
  			Edit->pos += XTextWidth(_plDefaultFont, buffer, strlen(buffer));
  			_plMoveCursor(Edit, oldpos, Edit->pos);
  			strcat(Edit->buffer, buffer);
  			Edit->loc++;
  }
}
