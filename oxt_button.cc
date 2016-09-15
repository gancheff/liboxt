// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_button.cc - button widget

#include "oxt_main.h"
#include "oxt_widget.h"
#include "oxt_button.h"
#include "oxt_internal.h"

void oxt_button::draw_button(int shape, int color) {
  if (type && (!inside && !t)) {
    XSetForeground(display, gc, standard);
    XFillRectangle(display, win, gc, 0, 0, w, h);
  } else {
    if (color) {
      XSetForeground(display, gc, color);
      XFillRectangle(display, win, gc, 0, 0, w, h);
    }
  
    draw_frame(shape, (type) ? SINGLE : DOUBLE, (color) ? 0 : 1, 0);
  }
    
  XSetForeground(display, gc, (shape == RAISED) ? black : white);
  XDrawString(display, win, gc, ((w)/2)-((XTextWidth(defaultfont, label, len))/2),
	(h/2)+(XTextWidth(defaultfont, "A", 1)/2), label, len);
}

void oxt_button::expose() {
  draw_button(((inside && pressed) || t) ? SUNKEN : RAISED, 0);
}

void oxt_button::button_press() {
  draw_button(SUNKEN, 0);
  pressed = 1;
}

void oxt_button::button_release() {
  if (inside && pressed) {

    if (callback_f) callback_f(this, data);
    if (!t && !type) draw_button(RAISED, oxt_color("#eeeeee"));
    else if (type && !t) draw_button(RAISED, 0);
  }

  pressed = 0;
}

void oxt_button::enter() {
  inside = 1;

  if ((pressed && t) || pressed) draw_button(SUNKEN, 0);
  else if (type && !t) draw_button(RAISED, 0);
  else if (!t) draw_button(RAISED, oxt_color("#eeeeee"));
}

void oxt_button::leave() {
  inside = 0;

  if ((pressed && !t) || (type && ((pressed && !t) || !t))) draw_button(RAISED, 0);
  else if (!t) draw_button(RAISED, standard);
}

void oxt_button::destroy() {
  delete_widget(this);
}

oxt_button::oxt_button(char *text) {
  label = text;
  len = strlen(text);
  parent = rootwindow;
  callback_f = 0;
  pressed = inside = type = t = x = y = 0;
  s = 1;
  w = XTextWidth(defaultfont, text, strlen(text))+10;
  h = XTextWidth(defaultfont, "A", 1)+12; // sue me!
  win = XCreateSimpleWindow(display, parent, x, y, w, h, 0, standard, standard);
  gc = XCreateGC(display, win, 0, 0);

  XSetWindowAttributes a;
  a.override_redirect = True;
  XChangeWindowAttributes(display, win, CWOverrideRedirect, &a);

  XSetForeground(display, gc, black);
  XSetFont(display, gc, defaultfont->fid);
  XSelectInput(display, win, ExposureMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask);

  register_widget(this);
}
