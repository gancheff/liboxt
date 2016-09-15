#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#define MAXWID 9800
#define NORMAL 0
#define AVOIDWM 1
#define RAISED 0
#define SUNKEN 1
#define ACTIVE 1
#define DEACTIVE 0

#define plColorBlack plGenColor(0x0000, 0x0000, 0x0000)
#define plColorWhite plGenColor(0xffff, 0xffff, 0xffff)
#define plColorRed plGenColor(0xffff, 0x0000, 0x0000)
#define plColorGreen plGenColor(0x0000, 0xffff, 0x0000)
#define plColorBlue plGenColor(0x0000, 0x0000, 0xffff)
#define plColorYellow plGenColor(0xffff, 0xffff, 0x0000)
#define plColorCyan plGenColor(0x0000, 0xffff, 0xffff)
#define plColorMagenta plGenColor(0xffff, 0x0000, 0xffff)

#define KEY_INSERT     65379
#define KEY_DELETE     65535
#define KEY_PGUP       65365
#define KEY_PGDOWN     65366
#define KEY_HOME       65360
#define KEY_END        65367
#define KEY_ENTER      65293
#define KEY_LEFTSHIFT  65505
#define KEY_RIGHTSHIFT 65506
#define KEY_CAPS       65509
#define KEY_BACKSPACE  65288
#define KEY_LEFTARROW  65361
#define KEY_RIGHTARROW 65363
#define KEY_UPARROW    65362
#define KEY_DOWNARROW  65364
#define KEY_TAB        65289

// identification
#define WINDOW 1
#define LABEL  2
#define BUTTON 3
#define EDIT   4

int _plScreen, _plDepth, _plNumber, _plQuit, _plClipCount, _plResKey;
unsigned long int _plStackCount, _plWhitePixel, _plBlackPixel, plStandardColor;
char *_plName;

Window _plRootWindow;
Display *_plDisplay;
Colormap _plColormap;
XFontStruct *_plDefaultFont;
XEvent _plEvent;
XRectangle _plClip[1024];

typedef struct plWidget_t {
  Window win, parent;
  GC gc;
  int x, y, w, h, status, active, id, pos, loc, shift, caps;
  char *buffer;
  struct plWidget_t *link;

  void (*plExpose) (struct plWidget_t *);
  void (*plKeyPress) (struct plWidget_t *);
  void (*plKeyRelease) (struct plWidget_t *);
  void (*plButtonPress) (struct plWidget_t *);
  void (*plButtonRelease) (struct plWidget_t *);
  void (*plDeleteWindow) (struct plWidget_t *);
  void (*plCallback) (struct plWidget_t *);
} plWidget;

plWidget *_plCurrent;
plWidget *_plStack[MAXWID];

void _plSetStatus(struct plWidget_t *widget, int stat);
void _plFrame(plWidget *widget, int background, int shape);
void _plMoveCursor(plWidget *Edit, int x1, int x2);

void plAddCallback(plWidget *widget, void *function);
void plLink(plWidget *wid1, plWidget *wid2);
void plQuit(void);
void plEventLoop(void);
unsigned long int plGenColor(int r, int g, int b);
plWidget *plWindowCreate(int x, int y, int w, int h, char *title, int opt);
plWidget *plLabelCreate(int x, int y, int w, int h, char *label, plWidget *location);
plWidget *plButtonCreate(int x, int y, int w, int h, char *label, plWidget *location);
plWidget *plEditCreate(int x, int y, int w, char *buffer, plWidget *location);
