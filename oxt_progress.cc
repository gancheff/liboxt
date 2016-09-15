// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_progress.cc - progress bar widget

#include "oxt_widget.h"
#include "oxt_progress.h"
#include "oxt_internal.h"

void oxt_progress::expose() {
  draw_frame(SUNKEN, SINGLE, 1, 0);
  if (p) draw_frame(RAISED, DOUBLE, 1, p);
}

void oxt_progress::set_progress(int np) {
  p = np;
  expose();
}

void oxt_progress::destroy() {
  delete_widget(this);
}

oxt_progress::oxt_progress(int wid, int hgh) {
  label = 0;
  len = x = y = p = 0;
  parent = rootwindow;
  w = wid;
  h = hgh;
  win = XCreateSimpleWindow(display, parent, x, y, w, h, 0, standard, standard);
  gc = XCreateGC(display, win, 0, 0);

  XSetWindowAttributes a;
  a.override_redirect = True;
  XChangeWindowAttributes(display, win, CWOverrideRedirect, &a);

  XSelectInput(display, win, ExposureMask | StructureNotifyMask);

  register_widget(this);
}
