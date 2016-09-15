// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_internal.h - internal declarations used by oxt

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

extern int screen, depth, number, quit;
extern char *app_basename;
extern Window rootwindow;
extern Colormap colormap;
extern XFontStruct *defaultfont;
extern XEvent event;
extern unsigned long black, white, standard, gray, darkgray;
extern Display *display;

extern oxt_widget *root, *curr;

void register_widget(oxt_widget *);
oxt_widget *get_widget(Window);
void delete_widget(oxt_widget *);
