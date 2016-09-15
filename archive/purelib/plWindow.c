#include "purelib.h"

void plWindowDelete(struct plWidget_t *);

plWidget *plWindowCreate(int x, int y, int w, int h, char *title, int opt) {
  plWidget *Window = malloc(sizeof(struct plWidget_t));
  XTextProperty plWindowTitle;
  XSetWindowAttributes plAttributes;
  XClassHint plClassHint;
  Atom _plWinAtom;

  if (w == 0 || h == 0) {
    fprintf(stderr, "[error] %s: window sized with zero!\n", _plName);
    exit(1);
  }

  Window->buffer = malloc(1024);
  Window->x = x;
  Window->y = y;
  Window->w = w;
  Window->h = h;
  Window->buffer = title;
  Window->parent = _plRootWindow;
  Window->id = WINDOW;

  Window->win = XCreateSimpleWindow(_plDisplay, _plRootWindow, x, y, w, h, 1, plColorBlack, plStandardColor);

  plClassHint.res_name = _plName;
  plClassHint.res_class = _plName;
  XSetClassHint(_plDisplay, Window->win, &plClassHint);

  Window->plExpose = 0;
  Window->plKeyPress = 0;
  Window->plKeyRelease = 0;
  Window->plButtonPress = 0;
  Window->plButtonRelease = 0;
  Window->plDeleteWindow = (void *)plWindowDelete;
  Window->plCallback = 0;

  if (opt == AVOIDWM) {
    plAttributes.override_redirect = True;
    XChangeWindowAttributes(_plDisplay, Window->win, CWOverrideRedirect, &plAttributes);
  }

  _plWinAtom = XInternAtom(_plDisplay, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(_plDisplay, Window->win, &_plWinAtom, 1);
  XStringListToTextProperty(&Window->buffer, 1, &plWindowTitle);
  XSetWMName(_plDisplay, Window->win, &plWindowTitle);
  XSelectInput(_plDisplay, Window->win, SubstructureNotifyMask | StructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);

  _plStack[_plStackCount] = Window;
  _plStackCount++;

  XMapWindow(_plDisplay, Window->win);

#ifdef DEBUG
  printf("[debug] window #%d created\n", _plStackCount);
#endif
  return Window;
}

void plWindowDelete(struct plWidget_t *Window) {
  printf("blah!\n");
}
