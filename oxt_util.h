// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_util.h - functions's declarations and other crap

#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define MAXFDS 64
#define MAXTIMERS 32

int oxt_inet_open(char *, int); 
void oxt_add_fd(int, void (*)(int));
void oxt_set_timer(int, int, void (*)(int));
void oxt_kill_timer(int);
