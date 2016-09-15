/*	oxt_timer.h -- timer include file
	vladimir ganchev */

#define TIMERS 16

typedef void (*timer)(void);

struct _timer_t {
  int avail, index, id, elapse;
  dword oldtimer;
  timer timerfunc;
};

void oxt_SetTimer(int, int, timer);
void oxt_KillTimer(int);
void oxt_HandleTimer(int);
