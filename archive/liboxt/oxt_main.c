/* 	oxt_main.c -- core functions
	vladimir ganchev */

#include "oxt_main.h"
#include "oxt_tree.h"
#include "oxt_internal.h"
#include "oxt_timer.h"

struct itimerval _itval;
dword _oldtimer;
extern dword _qwtimer;
extern struct _timer_t _timer[TIMERS];
extern struct _timer_t _zerotimer;
extern void _zerodbl(void);
int getline(char **, size_t *, FILE *);   // where da fuck is its declaration?!

void oxt_Init(int argc, char *argv[]) {
  int i, size = 0;
  char _getDisplay[1024], *buffer = malloc(1024), defconfig[1024];
  FILE *config;

  memset(_getDisplay, 0, 1024);
  memset(defconfig, 0, strlen(defconfig));
  if (getenv("DISPLAY") != 0) strcpy(_getDisplay, getenv("DISPLAY"));

  if (argc > 2)
    for (i=0; i<argc; i++)
      if (strcmp(argv[i], "-display") == 0)
        strcpy(_getDisplay, argv[i+1]);

  if (!(_display = XOpenDisplay(_getDisplay))) {
    fprintf(stderr, "%s: unable to open display: %s\n", argv[0], _getDisplay);
    exit(1);
  }

#ifdef DEBUG
  printf("[debug] %s: display is %p\n", argv[0], _display);
#endif

  _screen = DefaultScreen(_display);
  _rootwindow = RootWindow(_display, _screen);
  _depth = DefaultDepth(_display, _screen);
  _number = RootWindow(_display, _screen);
  _colormap = DefaultColormap(_display, _screen);
  _defaultfont = XLoadQueryFont(_display, "*helvetica*medium*-r*12*");
  _basename = argv[0];
  _active = 0;
  _dblclick.get1 = 0;
  _dblclick.get2 = 0;
  _qwtimer = 0UL;
  _oldtimer = 0UL;

  base = 0;
  root = malloc(100000);

  oxt_color.standard = oxt_GetColor(0xc1c1, 0xc1c1, 0xc1c1);
  oxt_color.black = oxt_GetColor(0x0000, 0x0000, 0x0000);
  oxt_color.white = oxt_GetColor(0xffff, 0xffff, 0xffff);

  memset(_timer, 0, sizeof(struct _timer_t) * 10);
  memset(&_itval, 0, sizeof(struct itimerval));

  signal(SIGALRM, oxt_HandleTimer);

  _itval.it_interval.tv_sec = 0;
  _itval.it_interval.tv_usec = 10;
  _itval.it_value.tv_sec = 0;
  _itval.it_value.tv_usec = 10;

  setitimer(ITIMER_REAL, &_itval, NULL);

  sprintf(defconfig, "%s/.liboxtrc", getenv("HOME"));

  if ((config = fopen(".liboxtrc", "r")) == NULL)
    config = fopen(defconfig, "r");

  if (config != NULL) {
    char bg[100], fg[100], fw[100], fs[100], ff[100], locbuff[1024];
    memset(locbuff, 0, 1024);
    memset(bg, 0, 100);
    memset(fg, 0, 100);
    memset(fw, 0, 100);
    memset(fs, 0, 100);
    memset(ff, 0, 100);

    while (getline(&buffer, &size, config) && !feof(config)) {
      oxt_ParseLine(buffer, "background color", bg);
      oxt_ParseLine(buffer, "foreground color", fg);
      oxt_ParseLine(buffer, "font size", fs);
      oxt_ParseLine(buffer, "font weight", fw);
      oxt_ParseLine(buffer, "font family", ff);
    }

    if (strcmp(fs, "") == 0) strcpy(fs, "12");
    if (strcmp(fw, "") == 0) strcpy(fw, "medium");
    if (strcmp(ff, "") == 0) strcpy(ff, "courier");

    sprintf(locbuff, "*%s*%s*-r*%s*", ff, fw, fs);
    if ((_defaultfont = XLoadQueryFont(_display, locbuff)) == 0) {
      printf("%s: Bad Font (%s)\n", argv[0], locbuff);
      exit(1);
    }
    if (strcmp(bg, "") != 0) oxt_color.standard = oxt_GetNamedColor(bg);
    if (strcmp(fg, "") != 0) oxt_color.black = oxt_GetNamedColor(fg);
  }
}

void oxt_AddCallback(oxt_Widget *widget, void *function) {
  widget->callback = function;
}

void oxt_Link(oxt_Widget *wid1, oxt_Widget *wid2) {
  wid1->link = wid2;
}

void oxt_Quit(void) {
  _quit = 1;
}

dword oxt_GetColor(int r, int g, int b) {
  XColor color;

  color.red = r;
  color.green = g;
  color.blue = b;

  XAllocColor(_display, _colormap, &color);
  return (color.pixel);
}

dword oxt_GetNamedColor(char *namedcolor) {
  XColor  color;

  XAllocNamedColor(_display, _colormap, namedcolor, &color, &color);
  return(color.pixel);
}

void oxt_Show(oxt_Widget *Window) {
  XMapWindow(_display, Window->win);
  XFlush(_display);
}

void oxt_ParseLine(char *in, char *name, char *out) {
  int i, j = 0;
  char *str = malloc(1024);

  while (1)
    if (in[j] == ' ' || in[j] == '\t') j++;
    else break;

  if (in[j] == '#') return;
  in += j;

  for (i=0; i<strlen(name); i++)
    if (tolower(name[i]) != tolower(in[i])) return;

  strcpy(str, strstr(in, ":"));
  j = 0;

  while (1)
    if (str[j] == ':' || str[j] == ' ' || str[j] == '\t') j++;

    else break;

  str += j;
  if (str[strlen(str)-1] == '\n') str[strlen(str)-1] = 0;

  strcpy(out, str);
}

void oxt_EventLoop(void) {
  oxt_Widget *widget;
  KeySym _key;

  while (!_quit) {
    XNextEvent(_display, &_event);

//    widget = _find_widget(_event.xany.window);

    widget = *(root+(_event.xany.window-base));

    switch(_event.type) {
      case Expose:
			if (widget->expose != 0) {
			  _clip[_ccount].x = _event.xexpose.x;
			  _clip[_ccount].y = _event.xexpose.y;
			  _clip[_ccount].width = _event.xexpose.width;
			  _clip[_ccount].height = _event.xexpose.height;
			  XSetClipRectangles(_display, widget->gc, 0, 0, _clip, _ccount+1, Unsorted);
			  _ccount = 0;
			
			  widget->expose(widget);

			  _clip[_ccount].x = 0;
			  _clip[_ccount].y = 0;
			  _clip[_ccount].width = widget->w;
			  _clip[_ccount].height = widget->h;
			  XSetClipRectangles(_display, widget->gc, 0, 0, _clip, 1, Unsorted);
			}

			break;
      case MotionNotify:
			if (widget->buttonmotion != 0)
			  widget->buttonmotion(widget);
			break;
      case ButtonPress:
			switch(_event.xbutton.button) {
			  case Button1:
					if (!_dblclick.get1) {
					  _dblclick.get1 = _event.xbutton.time;
					  oxt_SetTimer(1, 180, _zerodbl);
					} else {
					  _dblclick.get2 = _event.xbutton.time;
					  if ((_dblclick.get2)-(_dblclick.get1) < 180)
					    printf("Double click at window %ld\n", widget->win);
					  _dblclick.get1 = 0;
					  _dblclick.get2 = 0;
					}

					if (widget->buttonpress != 0)
					  widget->buttonpress(widget);
					break;
			  case Button2:
			  case Button3:
					if (_active != 0 && _active != widget) {
					  if (_active->status == 1) break;
					  _setstatus(_active, DEACTIVE);
					  _active = widget;
					  _setstatus(_active, ACTIVE);
					}
					break;
			}
			break;
      case ButtonRelease:
			if (widget->buttonrelease != 0)
			  widget->buttonrelease(widget);
			break;
      case KeyPress:
			_key = XKeycodeToKeysym(_display, _event.xkey.keycode, 0);

			if (_key == KEY_TAB        ||
			    _key == KEY_UPARROW    ||
			    _key == KEY_DOWNARROW) {  // handle widget changing
			  if (_active != 0) {
			    if (_active->status == 1) break;
			    _setstatus(_active, DEACTIVE);
			    if (_key == KEY_TAB        ||
				_key == KEY_DOWNARROW)
			      _active = (_active->next) ? _active->next : _active;
			    else
			      _active = (_active->prev) ? _active->prev : _active;
			    _setstatus(_active, ACTIVE);
			  }
			} else
			  if (_active->keypress != 0) _active->keypress(_active);

			break;
      case KeyRelease:
			if (_active->keyrelease != 0) _active->keyrelease(_active);
			break;
      case EnterNotify:
			if (widget->enter != 0) widget->enter(widget);
			break;
      case LeaveNotify:
			if (widget->leave != 0) widget->leave(widget);
			break;
      case ConfigureNotify:
			break;
      case ClientMessage:
			if (widget->delete != 0)
			  widget->delete(widget);
			break;
      case DestroyNotify:
			printf("window destroyed!\n");
			break;
    }
  }
}
