// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_util.cc - utility functions

#include <iostream>
#include <cstring>
#include "oxt_util.h"
using namespace std;

static int c = 0;
static int init = 0;
static void manage(int);
static struct itimerval itval;
static struct _timer_t {
	int i, t, c;
	void (*handler)(int);
} timer[MAXTIMERS];

static struct _fd_info_t {
	int fd;
	void (*handler)(int);
} fd_info[MAXFDS];


int oxt_inet_open(char *address, int port) {
  int d;
  struct sockaddr_in sa;

  d = socket(AF_INET, SOCK_STREAM, 0);

#ifdef DEBUG
  if (d >= 0) cout << app_basename << ": new socket: " << d << endl;
#endif

  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = *(long *)gethostbyname(address)->h_addr;
  sa.sin_port = htons(port);

  return (connect(d, (const struct sockaddr *)&sa, sizeof(sa)) == 0) ? d : -1;
}

void oxt_add_fd(int fd, void (*h_f)(int)) {
  if (c != MAXFDS && fd != -1 && h_f != 0) {
    fcntl(fd, F_SETFL, O_ASYNC);
    fcntl(fd, F_SETOWN, getpid());

    fd_info[c].fd = fd;
    fd_info[c].handler = h_f;

    c++;

#ifdef DEBUG
    cout << app_basename << ": registering file descriptor: " << fd << endl;
#endif
  }
  signal(SIGIO, manage);
}

void oxt_set_timer(int id, int time, void (*f)(int)) {
  if (!init) {
    memset(timer, -1, sizeof(struct _timer_t) * MAXTIMERS);
    memset(&itval, 0, sizeof(struct itimerval));

    signal(SIGALRM, manage);

    itval.it_interval.tv_sec = 0;
    itval.it_interval.tv_usec = 1;
    itval.it_value.tv_sec = 0;
    itval.it_value.tv_usec = 1;

    setitimer(ITIMER_REAL, &itval, NULL);
    init = 1;
  }

  for (int i=0; i<MAXTIMERS; i++)
    if (timer[i].i < 0) {
      timer[i].i = id;
      timer[i].t = time;
      timer[i].c = 0;
      timer[i].handler = f;
      break;
    }
}

void oxt_kill_timer(int id) {
  for (int i=0; i<MAXTIMERS; i++)
    if (timer[i].i == id) {
      memset(&timer[i], -1, sizeof(struct _timer_t));
      break;
    }
}

void manage(int n) {
  switch(n) {
    case SIGIO:
	int i, l;
	fd_set readfds;

	FD_ZERO(&readfds);
	for (i=0,l=0; i<c; i++, l = (fd_info[i].fd > l) ? fd_info[i].fd : l)
	  FD_SET(fd_info[i].fd, &readfds);

	select(l+1, &readfds, 0, 0, 0);

	for (i=0; i<c; i++)
	  if (FD_ISSET(fd_info[i].fd, &readfds)) {
	    if (fd_info[i].handler) fd_info[i].handler(fd_info[i].fd);
	    break;
	  }

	break;
    case SIGALRM:
	for (int i=0; i<MAXTIMERS; i++)
	  if (timer[i].i >= 0)
            if (++timer[i].c == timer[i].t) {
	      timer[i].handler(timer[i].i);
	      timer[i].c = 0;
	    }
  }
}
