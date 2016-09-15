// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_window.cc - window widget

#include "oxt_widget.h"
#include "oxt_window.h"
#include "oxt_internal.h"

void oxt_window::close() {
  XDestroyWindow(display, win);
}

void oxt_window::cmessage() {
  if (callback_f) callback_f(this, data);
  else close();
}

void oxt_window::destroy() {
  delete_widget(this);
}

void oxt_window::put(oxt_widget &widget) {
  XReparentWindow(display, widget.win, win, widget.getx(), widget.gety());
  widget.parent = win;
  XResizeWindow(display, win, widget.getw(), widget.geth());
}

oxt_window::oxt_window(int w_type, char *title) {
  label = title;
  len = strlen(title);
  parent = rootwindow;
  callback_f = 0;
  win = XCreateSimpleWindow(display, rootwindow, 0, 0, 100, 100, 1, black, standard);

  XClassHint h;
  h.res_name = app_basename;
  h.res_class = app_basename;
  XSetClassHint(display, win, &h);

  Atom w = XInternAtom(display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(display, win, &w, 1);

  XTextProperty t;
  XStringListToTextProperty(&label, 1, &t);
  XSetWMName(display, win, &t);

  if (w_type == AVOID_WM) {
    XSetWindowAttributes a;
    a.override_redirect = True;
    XChangeWindowAttributes(display, win, CWOverrideRedirect, &a);
  }

  XSelectInput(display, win, StructureNotifyMask | KeyPressMask);

  register_widget(this);
}
