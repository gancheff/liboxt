#include "purelib.h"

void plLabelExpose(struct plWidget_t *Label);

plWidget *plLabelCreate(int x, int y, int w, int h, char *label, plWidget *location) {
  plWidget *Label = malloc(sizeof(struct plWidget_t));

  if (w == 0 || h == 0) {
    fprintf(stderr, "[error] %s: window sized with zero!\n", _plName);
    exit(1);
  }

  Label->buffer = malloc(1024);
  Label->x = (x == 0) ? ((location->w)/2)-(w/2): x;
  Label->y = (y == 0) ? ((location->h)/2)-(h/2): y;
  Label->w = w;
  Label->h = h;
  Label->buffer = label;
  Label->parent = location->win;
  Label->id = LABEL;

  Label->win = XCreateSimpleWindow(_plDisplay, Label->parent, Label->x, Label->y, w, h, 0, plStandardColor, plStandardColor);

  Label->gc = XCreateGC(_plDisplay, Label->win, 0, 0);
  Label->plExpose = (void *)plLabelExpose;
  Label->plKeyPress = 0;
  Label->plKeyRelease = 0;
  Label->plButtonPress = 0;
  Label->plButtonRelease = 0;
  Label->plCallback = 0;

  XSetForeground(_plDisplay, Label->gc, plColorBlack);
  XSetFont(_plDisplay, Label->gc, _plDefaultFont->fid);
  XSelectInput(_plDisplay, Label->win, ExposureMask);

  _plStack[_plStackCount] = Label;
  _plStackCount++;

  XMapWindow(_plDisplay, Label->win);

#ifdef DEBUG
  printf("[debug] window #%d created\n", _plStackCount);
#endif
  return Label;
}

void plLabelExpose(struct plWidget_t *Label) {
#ifdef DEBUG
  printf("[debug] drawing label %s...\n", Label->buffer);
#endif
  XSetForeground(_plDisplay, Label->gc, plStandardColor);
  XFillRectangle(_plDisplay, Label->win, Label->gc, 0, 0, Label->w, Label->h);
  XSetForeground(_plDisplay, Label->gc, plColorBlack);
  XDrawString(_plDisplay, Label->win, Label->gc, ((Label->w)/2)-((XTextWidth(_plDefaultFont, Label->buffer, strlen(Label->buffer)))/2), ((Label->h)/2)+4, Label->buffer, strlen(Label->buffer));
}
