/*	oxt_internal.c -- internal functions used by oxt
	vladimir ganchev */

#include "oxt_main.h"
#include "oxt_internal.h"
#include "oxt_timer.h"
#include "oxt_tree.h"

int __timeout;
void _zerodbl(void);
void _blink(void);
extern void _drawcheck(struct oxt_Widget *Checkbox);

void _zerodbl(void) {
  _dblclick.get1 = 0;
  _dblclick.get2 = 0;
  oxt_KillTimer(1);
}

void _blink(void) {
  if (__timeout) {
    XSetForeground(_display, _active->gc, oxt_color.white);
    __timeout = 0;
  } else {
    XSetForeground(_display, _active->gc, oxt_color.black);
    __timeout = 1;
  }

  XDrawLine(_display, _active->win, _active->gc, _active->pos, 4, _active->pos, 16);
  XFlush(_display);  // blink, in the name of jesus!
}

void _drawcheck(oxt_Widget *Checkbox) {
  XSetForeground(_display, Checkbox->gc, oxt_color.black);

  XDrawLine(_display, Checkbox->win, Checkbox->gc, 9, 3, 4, 8);
  XDrawLine(_display, Checkbox->win, Checkbox->gc, 9, 4, 5, 8);
  XDrawLine(_display, Checkbox->win, Checkbox->gc, 9, 5, 5, 9);
  XDrawPoint(_display, Checkbox->win, Checkbox->gc, 3, 5);
  XFillRectangle(_display, Checkbox->win, Checkbox->gc, 3, 6, 2, 2);
}

void _movecursor(oxt_Widget *Edit, int x1, int x2) {
  if (x1 != 0) {
    XSetForeground(_display, Edit->gc, oxt_color.white);
    XDrawLine(_display, Edit->win, Edit->gc, x1, 4, x1, 16);
  }

  if (x2 != 0) {
    XSetForeground(_display, Edit->gc, oxt_color.black);
    XDrawLine(_display, Edit->win, Edit->gc, x2, 4, x2, 16);
  }
}

void _frame(oxt_Widget *widget, int background, int shape, int w, int h) {
  int width  = (w < 0) ? widget->w : w,
      height = (h < 0) ? widget->h : h;

  if (background != 0) {
    XSetForeground(_display, widget->gc, background);
    XFillRectangle(_display, widget->win, widget->gc, 0, 0, width, height);
  }

  if (shape == SUNKEN || shape == MINIMALSUNKEN)
    XSetForeground(_display, widget->gc, oxt_GetColor(0x6666, 0x6666, 0x6666));
  else
    XSetForeground(_display, widget->gc, oxt_GetColor(0xeeee, 0xeeee, 0xeeee));

  XDrawLine(_display, widget->win, widget->gc, 0, 0, width-1, 0);
  XDrawLine(_display, widget->win, widget->gc, 0, 0, 0, height-1);

  if (shape != MINIMALSUNKEN && shape != MINIMALRAISED) {
    if (shape == SUNKEN)
      XSetForeground(_display, widget->gc, oxt_GetColor(0x8888, 0x8888, 0x8888));
    else
      XSetForeground(_display, widget->gc, oxt_GetColor(0xdddd, 0xdddd, 0xdddd));

    XDrawLine(_display, widget->win, widget->gc, 1, 1, width-2, 1);
    XDrawLine(_display, widget->win, widget->gc, 1, 1, 1, height-2);


    if (shape == SUNKEN)
      XSetForeground(_display, widget->gc, oxt_GetColor(0xdddd, 0xdddd, 0xdddd));
    else
      XSetForeground(_display, widget->gc, oxt_GetColor(0x8888, 0x8888, 0x8888));

    XDrawLine(_display, widget->win, widget->gc, 2, height-2, width-2, height-2);
    XDrawLine(_display, widget->win, widget->gc, width-2, 2, width-2, height-1);
  }

  if (shape == SUNKEN || shape == MINIMALSUNKEN)
    XSetForeground(_display, widget->gc, oxt_GetColor(0xeeee, 0xeeee, 0xeeee));
  else
    XSetForeground(_display, widget->gc, oxt_GetColor(0x6666, 0x6666, 0x6666));

  XDrawLine(_display, widget->win, widget->gc, 1, height-1, width-1, height-1);
  XDrawLine(_display, widget->win, widget->gc, width-1, 1, width-1, height-1);
}

void _setstatus(oxt_Widget *widget, int stat) {
  if (!widget) return;
  switch(widget->id) {
    case WINDOW: break;
    case LABEL: break;
    case BUTTON:
		if (stat == ACTIVE) {
		  XSetForeground(_display, widget->gc, oxt_color.black);
		  widget->active = 1;
		} else {
		  XSetForeground(_display, widget->gc, oxt_color.standard);
		  widget->active = 0;
		}
		XDrawRectangle(_display, widget->win, widget->gc, 2, 2, (widget->w)-5, (widget->h)-5);
		break;
    case EDIT:
		if (stat == ACTIVE) {
		  widget->active = 1;
		  XSetForeground(_display, widget->gc, oxt_color.black);
 		  XDrawRectangle(_display, widget->win, widget->gc, 0, 0, (widget->w)-1, (widget->h)-1);
		  XDrawLine(_display, widget->win, widget->gc, widget->pos, 4, widget->pos, 16);
		  __timeout = 1;
		  if (!widget->blink) {
		    widget->blink = 1;
		    oxt_SetTimer(2, 400, _blink);
		  }
		} else {
		  widget->active = 0;
		  _frame(widget, 0, SUNKEN, -1, -1);
		  XSetForeground(_display, widget->gc, oxt_color.white);
		  XDrawLine(_display, widget->win, widget->gc, widget->pos, 4, widget->pos, 16);
		  if (widget->blink) widget->blink = 0;
		  oxt_KillTimer(2);
		}
		break;
    case CHECKBOX:
		if (stat == ACTIVE) {
		  XSetForeground(_display, widget->gc, oxt_color.black);
		  XDrawRectangle(_display, widget->win, widget->gc, 0, 0, 12, 12);
		  widget->active = 1;
		} else {
		  _frame(widget, 0, SUNKEN, 13, 13);
		  widget->active = 0;
		}
		break;
		
  }
}
