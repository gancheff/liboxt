#include "oxt_widget.h"
#include "oxt_label.h"
#include "oxt_internal.h"

void oxt_label::expose() {
  XDrawString(display, win, gc, ((w)/2)-((XTextWidth(defaultfont, label, len))/2),
		((h)/2)+(XTextWidth(defaultfont, "A", 1)/2), label, len);
}

void oxt_label::destroy() {
  delete_widget(this);
}

oxt_label::oxt_label(char *text) {
  label = text;
  len = strlen(text);
  parent = rootwindow;
  x = 0;
  y = 0;
  w = XTextWidth(defaultfont, text, strlen(text));
  h = 20;
  win = XCreateSimpleWindow(display, parent, x, y, w, h, 0, standard, standard);
  gc = XCreateGC(display, win, 0, 0);

  XSetWindowAttributes a;
  a.override_redirect = True;
  XChangeWindowAttributes(display, win, CWOverrideRedirect, &a);

  XSetForeground(display, gc, black);
  XSetFont(display, gc, defaultfont->fid);
  XSelectInput(display, win, ExposureMask | StructureNotifyMask);

  register_widget(this);
}
