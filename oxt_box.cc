// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_box.cc - box widget

#include "oxt_widget.h"
#include "oxt_box.h"
#include "oxt_internal.h"

void oxt_box::destroy() {
  delete_widget(this);
}

void oxt_box::put(oxt_widget &widget) {
  loc += (loc) ? spc : 0;
  XReparentWindow(display, widget.win, win,
		(t == HOR_BOX) ? loc : 0,
		(t == VER_BOX) ? loc : 0);

  widget.parent = win;

  loc += (t == HOR_BOX) ? widget.getw() : widget.geth();

  w = (t == HOR_BOX) ? loc : (widget.getw()>w) ? widget.getw() : w;
  h = (t == VER_BOX) ? loc : (widget.geth()>h) ? widget.geth() : h;

  XResizeWindow(display, win, w, h);
}

oxt_box::oxt_box(int type, int spacing, int border) {
  parent = rootwindow;
  t = type;
  spc = spacing;
  loc = x = y = 0;
  w = (border) ? border*2 : 1;
  h = (border) ? border*2 : 1;
  win = XCreateSimpleWindow(display, parent, x, y, w, h, border, standard, standard);

  XSetWindowAttributes a;
  a.override_redirect = True;
  XChangeWindowAttributes(display, win, CWOverrideRedirect, &a);

  XSelectInput(display, win, StructureNotifyMask);

  register_widget(this);
}
