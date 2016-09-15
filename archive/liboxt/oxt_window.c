/* 	oxt_window.c -- window widget
	vladimir ganchev */

#include "oxt_main.h"
#include "oxt_tree.h"
#include "oxt_internal.h"

void _window_delete(struct oxt_Widget *);

oxt_Widget *oxt_CreateWindow(int x, int y, int w, int h, char *title, int opt) {
  oxt_Widget *Window = malloc(sizeof(struct oxt_Widget));
  XTextProperty _title;
  XSetWindowAttributes _attributes;
  XClassHint _classhint;
  Atom _winatom;

  if (w == 0 || h == 0) {
    fprintf(stderr, "%s: window sized with zero!\n", _basename);
    exit(1);
  }

  Window->buffer = malloc(1024);
  Window->x = x;
  Window->y = y;
  Window->w = w;
  Window->h = h;
  Window->buffer = title;
  Window->parent = _rootwindow;
  Window->id = WINDOW;

  Window->win = XCreateSimpleWindow(_display, _rootwindow, x, y, w, h, 1, oxt_color.black, oxt_color.standard);

  _classhint.res_name = _basename;
  _classhint.res_class = _basename;
  XSetClassHint(_display, Window->win, &_classhint);

  Window->expose = 0;
  Window->keypress = 0;
  Window->keyrelease = 0;
  Window->buttonpress = 0;
  Window->buttonrelease = 0;
  Window->enter = 0;
  Window->leave = 0;
  Window->buttonmotion = 0;
  Window->delete = (void *)_window_delete;
  Window->callback = 0;

  if (opt == AVOIDWM) {
    _attributes.override_redirect = True;
    XChangeWindowAttributes(_display, Window->win, CWOverrideRedirect, &_attributes);
  }

  _winatom = XInternAtom(_display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(_display, Window->win, &_winatom, 1);
  XStringListToTextProperty(&Window->buffer, 1, &_title);
  XSetWMName(_display, Window->win, &_title);
  XSelectInput(_display, Window->win, SubstructureNotifyMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);

  _register_widget(Window);
  return Window;
}

void _window_delete(struct oxt_Widget *Window) {
  printf("%s: trying to delete window %s\n", _basename, Window->buffer);
}
