/* libsync
   vladimir ganchev */

#include <string.h>
#include "libsync.h"

static int c = 0;
static int init = 0;
static void manage(int);
static struct itimerval itval;

static struct _timer_t {
  int i, t, c;
  void (*handler)(int);
} _timer[MAXTIMERS];

static struct _fd_info_t {
  int fd;
  void (*handler)(int);
} _fd_info[MAXFDS];


int inet_open(char *address, int port) {
  int d;
  struct sockaddr_in sa;

  d = socket(AF_INET, SOCK_STREAM, 0);

  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = *(long *)gethostbyname(address)->h_addr;
  sa.sin_port = htons(port);

  return (connect(d, (const struct sockaddr *)&sa, sizeof(sa)) == 0) ? d : -1;
}

void add_fd(int fd, void (*h_f)(int)) {
  if (c != MAXFDS && fd != -1 && h_f != 0) {
    fcntl(fd, F_SETFL, O_ASYNC);
    fcntl(fd, F_SETOWN, getpid());

    _fd_info[c].fd = fd;
    _fd_info[c].handler = h_f;

    c++;

  }
  signal(SIGIO, manage);
}

void set_timer(int id, int time, void (*f)(int)) {
  int i;
  if (!init) {
    memset(_timer, -1, sizeof(struct _timer_t) * MAXTIMERS);
    memset(&itval, 0, sizeof(struct itimerval));

    signal(SIGALRM, manage);

    itval.it_interval.tv_sec = 0;
    itval.it_interval.tv_usec = 1;
    itval.it_value.tv_sec = 0;
    itval.it_value.tv_usec = 1;

    setitimer(ITIMER_REAL, &itval, NULL);
    init = 1;
  }

  for (i=0; i<MAXTIMERS; i++)
    if (_timer[i].i < 0) {
      _timer[i].i = id;
      _timer[i].t = time;
      _timer[i].c = 0;
      _timer[i].handler = f;
      break;
    }
}

void kill_timer(int id) {
  int i;
  for (i=0; i<MAXTIMERS; i++)
    if (_timer[i].i == id) {
      memset(&_timer[i], -1, sizeof(struct _timer_t));
      break;
    }
}

void manage(int n) {
  int i, l;
  fd_set readfds;

  switch(n) {
    case SIGIO:
	FD_ZERO(&readfds);
	for (i=0,l=0; i<c; i++) {
	  if (_fd_info[i].fd > l) l = _fd_info[i].fd;
	  FD_SET(_fd_info[i].fd, &readfds);
	}

	select(l+1, &readfds, 0, 0, 0);

	for (i=0; i<c; i++)
	  if (FD_ISSET(_fd_info[i].fd, &readfds)) {
	    if (_fd_info[i].handler) _fd_info[i].handler(_fd_info[i].fd);
	    break;
	  }

	break;
    case SIGALRM:
	for (i=0; i<MAXTIMERS; i++)
	  if (_timer[i].i >= 0)
            if (++_timer[i].c == _timer[i].t) {
	      _timer[i].handler(_timer[i].i);
	      _timer[i].c = 0;
	    }
  }
}
