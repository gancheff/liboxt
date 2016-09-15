// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_pixmap.cc - pixmap widget

#include "oxt_main.h"
#include "oxt_widget.h"
#include "oxt_pixmap.h"
#include "oxt_internal.h"
#include <X11/extensions/shape.h>
#include <X11/xpm.h>
using namespace std;

void oxt_pixmap::expose() {
  XCopyArea(display, pixmap, win, gc, 0, 0, w, h, 0, 0);
}

void oxt_pixmap::destroy() {
  delete_widget(this);
}

oxt_pixmap::oxt_pixmap(char **xpm) {
  cout << app_basename << ": create pixmap from xpm requested\n";
}

oxt_pixmap::oxt_pixmap(char *file) {
  label = 0;
  parent = rootwindow;
  x = y = 0;
  XpmAttributes xa;

  memset(&xa, 0, sizeof(XpmAttributes));
  xa.valuemask = (XpmVisual | XpmDepth | XpmColormap);
  xa.visual = DefaultVisual(display, DefaultScreen(display));
  xa.depth = depth;
  xa.colormap = colormap;

  XpmReadFileToPixmap(display, parent, file, &pixmap, &shape, &xa);

  w = xa.width;
  h = xa.height;

  win = XCreateSimpleWindow(display, parent, x, y, w, h, 0, standard, standard);
  XShapeCombineMask(display, win, ShapeBounding, 0, 0, shape, ShapeSet);
  gc = XCreateGC(display, pixmap, 0, 0);

  XSetWindowAttributes a;
  a.override_redirect = True;
  XChangeWindowAttributes(display, win, CWOverrideRedirect, &a);

  XSelectInput(display, win, ExposureMask);

  register_widget(this);
}
