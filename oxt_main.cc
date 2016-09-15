// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_main.cc - main functions

#include "oxt_main.h"
#include "oxt_widget.h"
#include "oxt_internal.h"
using namespace std;

int screen, depth, number, oxt_quit, g1;
char *app_basename;
Window rootwindow;
Colormap colormap;
XFontStruct *defaultfont;
XEvent event;
XRectangle clip;
unsigned long black, white, standard, gray, darkgray;
Display *display;
oxt_widget *curr, *root;

void oxt_libinit(int argc, char *argv[]) {
  char *get_display = 0;

  if (argc > 2) {
    for (int i=0; i<argc; i++)
      if (strcmp(argv[i], "-display") == 0)
        get_display = argv[i+1];
  } else get_display = getenv("DISPLAY");

  if (!(display = XOpenDisplay(get_display))) {
    cerr << argv[0] << ": unable to open display: " << get_display << endl;
    exit(1);
  }

#ifdef DEBUG
  cout << argv[0] << ": display is: " << get_display << endl;
#endif

  screen = DefaultScreen(display);
  rootwindow = RootWindow(display, screen);
  depth = DefaultDepth(display, screen);
  number = RootWindow(display, screen);
  colormap = DefaultColormap(display, screen);
  defaultfont = XLoadQueryFont(display, "*helvetica*medium*-r*12*");
  app_basename = argv[0];
  standard = oxt_color(0xd5d5, 0xd5d5, 0xd5d5);
  black = oxt_color(0x0000, 0x0000, 0x0000);
  white = oxt_color(0xffff, 0xffff, 0xffff);
  gray = oxt_color(0xeeee, 0xeeee, 0xeeee);
  darkgray = oxt_color(0x8888, 0x8888, 0x8888);
  root = curr = 0;
  g1 = 0;
}

unsigned long oxt_color(int r, int g, int b) {
  XColor color;

  color.red = r;
  color.green = g;
  color.blue = b;

  XAllocColor(display, colormap, &color);
  return (color.pixel);
}

unsigned long oxt_color(char *namedcolor) {
  XColor  color;

  XAllocNamedColor(display, colormap, namedcolor, &color, &color);
  return(color.pixel);
}

void oxt_run() {
  oxt_widget *widget, *temp = root;

  while (!oxt_quit) {
    XNextEvent(display, &event);
    if (!(widget = get_widget(event.xany.window))) continue;

    switch(event.type) {
      case Expose:
	clip.x = event.xexpose.x;
	clip.y = event.xexpose.y;
	clip.width = event.xexpose.width;
	clip.height = event.xexpose.height;
	XSetClipRectangles(display, widget->gc, 0, 0, &clip, 1, Unsorted);
	widget->expose();
	clip.x = 0;
	clip.y = 0;
	clip.width = widget->getw();
	clip.height = widget->geth();
	XSetClipRectangles(display, widget->gc, 0, 0, &clip, 1, Unsorted);
	break;
      case MotionNotify: widget->button_motion(); break;
      case ButtonPress:
	if (!g1) g1 = event.xbutton.time;
	else {
	  if (((event.xbutton.time)-g1) < 180)
	    cout << "double click at window " << widget->get_label() << endl;
	  g1 = 0;
	}

	widget->button_press(); break;
      case ButtonRelease: widget->button_release(); break;
      case KeyPress: temp = temp->next; break;
      case KeyRelease: break;
      case EnterNotify: widget->enter(); break;
      case LeaveNotify: widget->leave(); break;
      case ConfigureNotify: break;
      case ClientMessage: widget->cmessage(); break;
      case DestroyNotify: widget->destroy(); break;
    }
  }
}
