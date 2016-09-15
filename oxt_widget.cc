// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_widget.cc - base class

#include "oxt_main.h"
#include "oxt_widget.h"
#include "oxt_internal.h"
using namespace std;

oxt_widget::oxt_widget() {
  if (!display) {
    cout << "oxt warning: initializing with zero argc and argv\n";
    oxt_libinit(0, 0);
  }
}

void oxt_widget::draw_frame(int shape, int border, int fill, int fw) {
  int nw = (fw) ? fw+1 : w;
  if (fill) {
    XSetForeground(display, gc, (shape == RAISED) ? standard : oxt_color(0x8888, 0x8888, 0x8888));
    XFillRectangle(display, win, gc, 1, 1, nw-2, h-2);
  }

  XSetForeground(display, gc, (shape == RAISED) ? white : black);
  XDrawLine(display, win, gc, 1, 0, nw-3, 0);
  XDrawLine(display, win, gc, 0, 1, 0, h-2);

  XSetForeground(display, gc, (shape == RAISED) ? black : white);
  XDrawLine(display, win, gc, 2, h-1, nw-2, h-1);
  XDrawLine(display, win, gc, nw-1, 1, nw-1, h-2);

  if (border == DOUBLE) {
    XSetForeground(display, gc, (shape == RAISED) ? gray : darkgray);
    XDrawLine(display, win, gc, 1, 1, nw-2, 1);
    XDrawLine(display, win, gc, 1, 1, 1, h-2);
    XSetForeground(display, gc, (shape == RAISED) ? darkgray : gray);
    XDrawLine(display, win, gc, 2, h-2, nw-2, h-2);
    XDrawLine(display, win, gc, nw-2, 2, nw-2, h-2);
  }
}

void oxt_widget::callback(void (*f)(oxt_widget *, void*), void *d) {
  callback_f = f;
  data = d;
}

void oxt_widget::toggle() {
  t = (t) ? 0 : 1;
}

int oxt_widget::is_toggle() {
  return t;
}

void oxt_widget::set_label(char *newlabel) {
  label = newlabel;
  len = strlen(newlabel);
  w = XTextWidth(defaultfont, newlabel, len)+10;
  XResizeWindow(display, win, w, h);
  XSetForeground(display, gc, standard);
  XFillRectangle(display, win, gc, 0, 0, w, h);
  XSetForeground(display, gc, black);
  expose();
  XFlush(display);
}

int oxt_widget::get_status() { return s; }
char *oxt_widget::get_label() { return label; }
    
int oxt_widget::set_type(int new_type) {
  if (new_type == GET_TYPE) return type;
  return (type = new_type);
}

void oxt_widget::set_size(int i, int j) {
  w = i;
  h = j;

  XResizeWindow(display, win, w, h);
}

void oxt_widget::set_pos(int i, int j) {
  x = i;
  y = j;

  XMoveWindow(display, win, x, y);
}

void oxt_widget::appear() { XMapWindow(display, win); XFlush(display);}
void oxt_widget::hide() { XUnmapWindow(display, win); XFlush(display);}
int oxt_widget::getx() { return x; }
int oxt_widget::gety() { return y; }
int oxt_widget::getw() { return w; }
int oxt_widget::geth() { return h; }
