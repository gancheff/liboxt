/* libsync
   vladimir ganchev */

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define MAXFDS 64
#define MAXTIMERS 32

int inet_open(char *, int); 
void add_fd(int, void (*)(int));
void set_timer(int, int, void (*)(int));
void kill_timer(int);
