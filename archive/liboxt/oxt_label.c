/*	oxt_label.c -- label widget
	vladimir ganchev */

#include "oxt_main.h"
#include "oxt_tree.h"
#include "oxt_internal.h"

void _label_expose(struct oxt_Widget *Label);

oxt_Widget *oxt_CreateLabel(int x, int y, int w, int h, char *label, oxt_Widget *location) {
  oxt_Widget *Label = malloc(sizeof(struct oxt_Widget));

  if (w == 0 || h == 0) {
    fprintf(stderr, "%s: window sized with zero!\n", _basename);
    exit(1);
  }

  Label->buffer = malloc(1024);
  Label->x = (x < 0) ? ((location->w)/2)-(w/2): x;
  Label->y = (y < 0) ? ((location->h)/2)-(h/2): y;
  Label->w = w;
  Label->h = h;
  Label->buffer = label;
  Label->parent = location->win;
  Label->id = LABEL;

  Label->win = XCreateSimpleWindow(_display, Label->parent, Label->x, Label->y, w, h, 0, oxt_color.standard, oxt_color.standard);

  Label->gc = XCreateGC(_display, Label->win, 0, 0);
  Label->expose = (void *)_label_expose;
  Label->keypress = 0;
  Label->keyrelease = 0;
  Label->buttonpress = 0;
  Label->buttonrelease = 0;
  Label->enter = 0;
  Label->leave = 0;
  Label->buttonmotion = 0;
  Label->callback = 0;

  XSetForeground(_display, Label->gc, oxt_color.black);
  XSetFont(_display, Label->gc, _defaultfont->fid);
  XSelectInput(_display, Label->win, ExposureMask);

  _register_widget(Label);
  return Label;
}

void _label_expose(struct oxt_Widget *Label) {
#ifdef DEBUG
  printf("[debug] drawing label %s...\n", Label->buffer);
#endif
  XSetForeground(_display, Label->gc, oxt_color.standard);
  XFillRectangle(_display, Label->win, Label->gc, 0, 0, Label->w, Label->h);
  XSetForeground(_display, Label->gc, oxt_color.black);
  XDrawString(_display, Label->win, Label->gc, ((Label->w)/2)-((XTextWidth(_defaultfont, Label->buffer, strlen(Label->buffer)))/2), ((Label->h)/2)+4, Label->buffer, strlen(Label->buffer));
}
