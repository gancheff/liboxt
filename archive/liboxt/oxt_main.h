/*	oxt_main.h -- base include file
	vladimir ganchev */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#define NORMAL		0
#define AVOIDWM         1
#define RAISED		0
#define SUNKEN		1
#define MINIMALRAISED	2
#define MINIMALSUNKEN	3
#define ACTIVE		1
#define DEACTIVE	0

#define WINDOW		1
#define LABEL		2
#define BUTTON		3
#define EDIT		4
#define CHECKBOX	5

#define Red	oxt_GetColor(0xffff, 0x0000, 0x0000)
#define Green	oxt_GetColor(0x0000, 0xffff, 0x0000)
#define Blue	oxt_GetColor(0x0000, 0x0000, 0xffff)
#define Yellow	oxt_GetColor(0xffff, 0xffff, 0x0000)
#define Cyan	oxt_GetColor(0x0000, 0xffff, 0xffff)
#define Magenta	oxt_GetColor(0xffff, 0x0000, 0xffff)

#define KEY_INSERT	65379
#define KEY_DELETE	65535
#define KEY_PGUP	65365
#define KEY_PGDOWN	65366
#define KEY_HOME	65360
#define KEY_END		65367
#define KEY_ENTER	65293
#define KEY_LEFTSHIFT	65505
#define KEY_RIGHTSHIFT	65506
#define KEY_CAPS	65509
#define KEY_BACKSPACE	65288
#define KEY_LEFTARROW	65361
#define KEY_RIGHTARROW	65363
#define KEY_UPARROW	65362
#define KEY_DOWNARROW	65364
#define KEY_TAB		65289
#define KEY_LEFTCTRL	65507
#define KEY_RIGHTCTRL	65508
#define KEY_LEFTALT	65513
#define KEY_RIGHTALT	65514
#define KEY_F1		65470
#define KEY_F2		65471
#define KEY_F3		65472
#define KEY_F4		65473
#define KEY_F5		65474
#define KEY_F6		65475
#define KEY_F7		65476
#define KEY_F8		65477
#define KEY_F9		65478
#define KEY_F10		65479
#define KEY_F11		654710
#define KEY_F12		654711

typedef unsigned long dword;

dword base;

struct coxt_t {
  int sx, sx1, sx2, x, w;
};

struct oxt_color_t {
  dword black, white, standard;
} oxt_color;

typedef struct oxt_Widget {
  Window win, parent;
  GC gc;
  int x, y, w, h, status, active, content, id, pos, loc, shift, caps, blink;
  char *buffer, *clipboard;
  struct oxt_Widget *link, *next, *prev;
  struct coxt_t cb;

  void (*expose) 	(struct oxt_Widget *);
  void (*keypress) 	(struct oxt_Widget *);
  void (*keyrelease) 	(struct oxt_Widget *);
  void (*buttonpress) 	(struct oxt_Widget *);
  void (*buttonrelease) (struct oxt_Widget *);
  void (*buttonmotion) 	(struct oxt_Widget *);
  void (*enter) 	(struct oxt_Widget *);
  void (*leave) 	(struct oxt_Widget *);
  void (*delete) 	(struct oxt_Widget *);
  void (*callback) 	(struct oxt_Widget *);
} oxt_Widget;

void **root;
void oxt_Init(int, char **);
void oxt_AddCallback(oxt_Widget *, void *);
void oxt_Link(oxt_Widget *, oxt_Widget *);
void oxt_Quit(void);
void oxt_Show(oxt_Widget *);
void oxt_EventLoop(void);
void oxt_ParseLine(char *, char *, char *);
dword oxt_GetColor(int, int, int);
dword oxt_GetNamedColor(char *);

oxt_Widget *oxt_CreateWindow(int, int, int, int, char *, int);
oxt_Widget *oxt_CreateLabel(int, int, int, int, char *, oxt_Widget *);
oxt_Widget *oxt_CreateButton(int, int, int, int, char *, oxt_Widget *);
oxt_Widget *oxt_CreateCheckbox(int, int, char *, oxt_Widget *);
oxt_Widget *oxt_CreateEdit(int, int, int, char *, oxt_Widget *);
