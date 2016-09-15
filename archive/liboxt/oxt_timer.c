/*	oxt_timer.c -- timer support
	vladimir ganchev
	sjamaan <sjamaan@mail.orbitel.bg> */

#include "oxt_main.h"
#include "oxt_internal.h"
#include "oxt_timer.h"

dword _qwtimer;
struct _timer_t _timer[TIMERS];
struct _timer_t _zerotimer = { (int)0, (int)0, (int)0, (int)0, (dword)0UL, (timer)NULL };

void oxt_SetTimer(int id, int elapse, timer function) {
  int i;

  for (i=0; i<10; i++)
    if (!_timer[i].avail) {
      _timer[i].avail = 1;
      _timer[i].index = 1;
      _timer[i].id = id;
      _timer[i].elapse = elapse;
      _timer[i].oldtimer = _qwtimer;
      _timer[i].timerfunc = function;
      return;
    }
}

void oxt_KillTimer(int id) {
  int i;

  for (i=0; i<10; i++)
    if (_timer[i].id == id) {
      _timer[i] = _zerotimer;
      return;
    }
}

void oxt_HandleTimer(int j) {
  int i;

  signal(SIGALRM, SIG_IGN);
  _qwtimer += 10;

  for (i=0; i<10; i++)
    if (_timer[i].avail && !(_qwtimer % _timer[i].elapse) &&
        (_qwtimer - _timer[i].oldtimer) >= _timer[i].elapse) {
      _timer[i].timerfunc();
      _timer[i].oldtimer = _qwtimer;
    }

  signal(SIGALRM, oxt_HandleTimer);
}
