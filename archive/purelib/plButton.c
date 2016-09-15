#include "purelib.h"

void plButtonExpose(struct plWidget_t *Button);
void plButtonPress(struct plWidget_t *Button);
void plButtonRelease(struct plWidget_t *Button);
void plKeyPress(struct plWidget_t *Button);
void plKeyRelease(struct plWidget_t *Button);

plWidget *plButtonCreate(int x, int y, int w, int h, char *label, plWidget *location) {
  plWidget *Button = malloc(sizeof(struct plWidget_t));

  if (w == 0 || h == 0) {
    fprintf(stderr, "[error] %s: window sized with zero!\n", _plName);
    exit(1);
  }

  Button->link = 0;
  Button->buffer = malloc(1024);
  Button->x = (x < 0) ? ((location->w)/2)-(w/2): x;
  Button->y = (y < 0) ? ((location->h)/2)-(h/2): y;
  Button->w = w;
  Button->h = h;
  Button->buffer = label;
  Button->parent = location->win;
  Button->status = 0;
  Button->id = BUTTON;

  Button->win = XCreateSimpleWindow(_plDisplay, Button->parent, Button->x, Button->y, w, h, 0, plColorBlack, plStandardColor);

  Button->gc = XCreateGC(_plDisplay, Button->win, 0, 0);
  Button->plExpose = (void *)plButtonExpose;
  Button->plKeyPress = (void *)plKeyPress;
  Button->plKeyRelease = (void *)plKeyRelease;
  Button->plButtonPress = (void *)plButtonPress;
  Button->plButtonRelease = (void *)plButtonRelease;
  Button->plCallback = 0;

  XSetFont(_plDisplay, Button->gc, _plDefaultFont->fid);
  XSelectInput(_plDisplay, Button->win, ExposureMask | EnterWindowMask | LeaveWindowMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask);

  if (_plCurrent == 0) {
    _plCurrent = Button;
    Button->active = 1;
  }

  _plStack[_plStackCount] = Button;
  _plStackCount++;

  XMapWindow(_plDisplay, Button->win);

#ifdef DEBUG
  printf("[debug] window #%d created\n", _plStackCount);
#endif

  return Button;
}

void plButtonExpose(struct plWidget_t *Button) {
  int twidth = XTextWidth(_plDefaultFont, Button->buffer, strlen(Button->buffer));

  if (twidth > (Button->w)-6) {
    XResizeWindow(_plDisplay, Button->win, (Button->w)+(twidth-((Button->w)-6)), Button->h);
    Button->w = (Button->w)+(twidth-((Button->w)-6));
  }

  _plFrame(Button, plStandardColor, RAISED);
  XSetForeground(_plDisplay, Button->gc, plColorBlack);
  XDrawString(_plDisplay, Button->win, Button->gc, ((Button->w)/2)-(twidth)/2,
								((Button->h)/2)+3, Button->buffer, strlen(Button->buffer));
  if (Button->active)
    _plSetStatus(Button, ACTIVE);
}

void plButtonPress(struct plWidget_t *Button) {
  if (_plCurrent != Button) {
    if (_plCurrent->status == 1) return;
    _plCurrent->active = 0;
    _plSetStatus(_plCurrent, DEACTIVE);
    _plCurrent = Button;
    Button->active = 1;
  }

  _plFrame(Button, plGenColor(0xbbbb, 0xbbbb, 0xbbbb), SUNKEN);
  XSetForeground(_plDisplay, Button->gc, plColorBlack);
  XDrawString(_plDisplay, Button->win, Button->gc, (((Button->w)/2)-(XTextWidth(_plDefaultFont, Button->buffer,
											strlen(Button->buffer))/2))+1,
								((Button->h)/2)+4, Button->buffer, strlen(Button->buffer));
  _plSetStatus(Button, ACTIVE);

  Button->status = 1;
}

void plButtonRelease(struct plWidget_t *Button) {
  if (_plEvent.xbutton.x < Button->w && _plEvent.xbutton.x > 0
	&& _plEvent.xbutton.y < Button->h && _plEvent.xbutton.y > 0 && Button->status == 1) {
    if (Button->plCallback != 0)
      Button->plCallback(Button);
    Button->plExpose(Button);
  }

  Button->status = 0;
}

void plKeyPress(struct plWidget_t *Button) {
  KeySym plKey = XKeycodeToKeysym(_plDisplay, _plEvent.xkey.keycode, 0);

  switch(plKey) {
    case 32:
    case KEY_ENTER:
			_plResKey = 1;
			Button->plButtonPress(Button);
			break;
  }
}

void plKeyRelease(struct plWidget_t *Button) {
  KeySym plKey = XKeycodeToKeysym(_plDisplay, _plEvent.xkey.keycode, 0);

  switch(plKey) {
    case 32:
    case KEY_ENTER:
			_plResKey = 0;
			if (Button->plCallback != 0) Button->plCallback(Button);
			Button->plExpose(Button);
			Button->status = 0;
			break;
  }
}

