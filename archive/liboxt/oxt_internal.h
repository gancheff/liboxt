/*	oxt_internal.h -- internal include file
	vladimir ganchev */

int _screen, _depth, _number, _quit, _ccount, _rkey;
char *_basename;

struct dbl_t {
  dword get1, get2;
} _dblclick;

Window _rootwindow;
Display *_display;
Colormap _colormap;
XFontStruct *_defaultfont;
XEvent _event;
XRectangle _clip[1024];

void _drawcheck(oxt_Widget *);
void _setstatus(oxt_Widget *, int);
void _frame(oxt_Widget *, int, int, int, int);
void _movecursor(oxt_Widget *, int, int);
