#include "purelib.h"

void plCreate(int argc, char *argv[]) {
  int i;
  char _getDisplay[1024];

  memset(_getDisplay, 0, 1024);
  if (getenv("DISPLAY") != 0) strcpy(_getDisplay, getenv("DISPLAY"));

  if (argc > 2)
    for (i=0; i<argc; i++)
      if (strcmp(argv[i], "-display") == 0)
        strcpy(_getDisplay, argv[i+1]);

  if (!(_plDisplay = XOpenDisplay(_getDisplay))) {
    fprintf(stderr, "%s: unable to open display: %s\n", argv[0], _getDisplay);
    exit(1);
  }

#ifdef DEBUG
  printf("[debug] %s: display is %p\n", argv[0], _plDisplay);
#endif

  _plScreen = DefaultScreen(_plDisplay);
  _plRootWindow = RootWindow(_plDisplay, _plScreen);
  _plDepth = DefaultDepth(_plDisplay, _plScreen);
  _plNumber = RootWindow(_plDisplay, _plScreen);
  _plColormap = DefaultColormap(_plDisplay, _plScreen);
  _plWhitePixel = WhitePixel(_plDisplay, _plScreen);
  _plBlackPixel = BlackPixel(_plDisplay, _plScreen);
  _plDefaultFont = XLoadQueryFont(_plDisplay, "*courier*medium*-r*12*");
  _plStackCount = 0;
  _plName = argv[0];
  _plCurrent = 0;
  plStandardColor = plGenColor(0xcfcf, 0xcfcf, 0xd0d0);

//  memset(_plStack, 0, sizeof(_plStack));

#ifdef DEBUG
  printf("[debug] %s: screen: %d, number %d, white %d, black %d\n", argv[0],
								    _plScreen,
								    _plNumber,
								    _plWhitePixel,
								    _plBlackPixel);
#endif
}

void plAddCallback(plWidget *widget, void *function) {
  widget->plCallback = function;
}

void plLink(plWidget *wid1, plWidget *wid2) {
  wid1->link = wid2;
}

void plQuit(void) {
  _plQuit = 1;
}

void plEventLoop(void) {
  int i;

  while (!_plQuit) {
    XNextEvent(_plDisplay, &_plEvent);

    for (i=0; i<_plStackCount; i++)
      if (_plEvent.xany.window == _plStack[i]->win) break;

    switch(_plEvent.type) {
      case Expose:
			if (_plStack[i]->plExpose != 0) {
			  _plClip[_plClipCount].x = _plEvent.xexpose.x;
			  _plClip[_plClipCount].y = _plEvent.xexpose.y;
			  _plClip[_plClipCount].width = _plEvent.xexpose.width;
			  _plClip[_plClipCount].height = _plEvent.xexpose.height;
			  XSetClipRectangles(_plDisplay, _plStack[i]->gc, 0, 0, _plClip, _plClipCount+1, Unsorted);
			  _plClipCount = 0;
			
			  _plStack[i]->plExpose(_plStack[i]);

			  _plClip[_plClipCount].x = 0;
			  _plClip[_plClipCount].y = 0;
			  _plClip[_plClipCount].width = _plStack[i]->w;
			  _plClip[_plClipCount].height = _plStack[i]->h;
			  XSetClipRectangles(_plDisplay, _plStack[i]->gc, 0, 0, _plClip, 1, Unsorted);
			}

			break;
      case ButtonPress:
			switch(_plEvent.xbutton.button) {
			  case Button1:
					if (_plStack[i]->plButtonPress != 0)
					  _plStack[i]->plButtonPress(_plStack[i]);
					break;
			  case Button2:
			  case Button3:
					if (_plCurrent != 0 && _plCurrent != _plStack[i]) {
					  if (_plCurrent->status == 1) break;
					  _plCurrent->active = 0;
					  _plSetStatus(_plCurrent, DEACTIVE);
					  _plCurrent = _plStack[i];
					  _plCurrent->active = 1;
					  _plSetStatus(_plCurrent, ACTIVE);
					}
					break;
			}
			break;
      case ButtonRelease:
			if (_plStack[i]->plButtonRelease != 0)
			  _plStack[i]->plButtonRelease(_plStack[i]);
			break;
      case KeyPress:
			if (XKeycodeToKeysym(_plDisplay, _plEvent.xkey.keycode, 0) == KEY_TAB) {  // handle widget changing
			  if (_plCurrent != 0) {
			    if (_plCurrent->status == 1) break;
			    _plCurrent->active = 0;
			    _plSetStatus(_plCurrent, DEACTIVE);
			    for (i=0; i<_plStackCount; i++)
			      if (_plCurrent->win == _plStack[i]->win) break;
			    if (i+1 >= _plStackCount) i = 0;
			    else i++;
			    _plCurrent = _plStack[i];
			    _plCurrent->active = 1;
			    _plSetStatus(_plCurrent, ACTIVE);
			  }
			} else
			  if (_plCurrent->plKeyPress != 0) _plCurrent->plKeyPress(_plCurrent);
			break;
      case KeyRelease:
			if (_plCurrent->plKeyRelease != 0) _plCurrent->plKeyRelease(_plCurrent);
			break;
      case EnterNotify:
			if (_plResKey == 1) break;
			if (_plStack[i]->status == 1) _plStack[i]->plButtonPress(_plStack[i]);
			break;
      case LeaveNotify:
			if (_plResKey == 1) break;
			if (_plStack[i]->status == 1) _plStack[i]->plExpose(_plStack[i]);
			break;
      case ConfigureNotify:
			break;
      case ClientMessage:
			if (_plStack[i]->plDeleteWindow != 0)
			  _plStack[i]->plDeleteWindow(_plStack[i]);
			break;
    }
  }
}

void _plMoveCursor(plWidget *Edit, int x1, int x2) {
  if (x1 != 0) {
    XSetForeground(_plDisplay, Edit->gc, plColorWhite);
    XDrawLine(_plDisplay, Edit->win, Edit->gc, x1, 3, x1, 15);
  }

  if (x2 != 0) {
    XSetForeground(_plDisplay, Edit->gc, plColorBlack);
    XDrawLine(_plDisplay, Edit->win, Edit->gc, x2, 3, x2, 15);
  }
}

void _plFrame(plWidget *widget, int background, int shape) {

  if (background != 0) {
    XSetForeground(_plDisplay, widget->gc, background);
    XFillRectangle(_plDisplay, widget->win, widget->gc, 0, 0, widget->w, widget->h);
  }

  if (shape == SUNKEN)
    XSetForeground(_plDisplay, widget->gc, plGenColor(0x6666, 0x6666, 0x6666));
  else
    XSetForeground(_plDisplay, widget->gc, plGenColor(0xeeee, 0xeeee, 0xeeee));

  XDrawLine(_plDisplay, widget->win, widget->gc, 0, 0, widget->w, 0);
  XDrawLine(_plDisplay, widget->win, widget->gc, 0, 0, 0, widget->h);

  if (shape == SUNKEN)
    XSetForeground(_plDisplay, widget->gc, plGenColor(0x8888, 0x8888, 0x8888));
  else
    XSetForeground(_plDisplay, widget->gc, plGenColor(0xdddd, 0xdddd, 0xdddd));

  XDrawLine(_plDisplay, widget->win, widget->gc, 1, 1, (widget->w)-2, 1);
  XDrawLine(_plDisplay, widget->win, widget->gc, 1, 1, 1, (widget->h)-2);

  if (shape == SUNKEN)
    XSetForeground(_plDisplay, widget->gc, plGenColor(0xeeee, 0xeeee, 0xeeee));
  else
    XSetForeground(_plDisplay, widget->gc, plGenColor(0x6666, 0x6666, 0x6666));

  XDrawLine(_plDisplay, widget->win, widget->gc, 1, (widget->h)-1, (widget->w)-1, (widget->h)-1);
  XDrawLine(_plDisplay, widget->win, widget->gc, (widget->w)-1, 1, (widget->w)-1, widget->h);

  if (shape == SUNKEN)
    XSetForeground(_plDisplay, widget->gc, plGenColor(0xdddd, 0xdddd, 0xdddd));
  else
    XSetForeground(_plDisplay, widget->gc, plGenColor(0x8888, 0x8888, 0x8888));

  XDrawLine(_plDisplay, widget->win, widget->gc, 2, (widget->h)-2, (widget->w)-2, (widget->h)-2);
  XDrawLine(_plDisplay, widget->win, widget->gc, (widget->w)-2, 2, (widget->w)-2, widget->h);
}

unsigned long int plGenColor(int r, int g, int b) {
  XColor color;

  color.red = r;
  color.green = g;
  color.blue = b;

  XAllocColor(_plDisplay, _plColormap, &color);
  return (color.pixel);
}

void plShow(plWidget *Window) {
  XMapWindow(_plDisplay, Window->win);
  XFlush(_plDisplay);
}

void _plSetStatus(struct plWidget_t *widget, int stat) {
  switch(widget->id) {
    case WINDOW: break;
    case LABEL: break;
    case BUTTON:
		if (stat == ACTIVE) XSetForeground(_plDisplay, widget->gc, plColorBlack);
		else XSetForeground(_plDisplay, widget->gc, plStandardColor);
		XDrawRectangle(_plDisplay, widget->win, widget->gc, 2, 2, (widget->w)-5, (widget->h)-5);
		break;
    case EDIT:
		if (stat == ACTIVE) {
		  XSetForeground(_plDisplay, widget->gc, plColorBlack);
 		  XDrawRectangle(_plDisplay, widget->win, widget->gc, 0, 0, (widget->w)-1, (widget->h)-1);
		  XDrawLine(_plDisplay, widget->win, widget->gc, widget->pos, 3, widget->pos, 15);
		} else {
		  _plFrame(widget, 0, SUNKEN);
		  XSetForeground(_plDisplay, widget->gc, plColorWhite);
		  XDrawLine(_plDisplay, widget->win, widget->gc, widget->pos, 3, widget->pos, 15);
		}
		break;
  }
}
