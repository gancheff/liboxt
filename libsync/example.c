#include <stdio.h>
#include <stdlib.h>
#include "libsync.h"

void read_data(int fd) {
  char b[1024];
  int size = read(fd, b, 1024);
  printf("size: %d\n", size);
  write(1, b, size);
}

void timer(int t) {
  printf("timer %d activated\n", t);
}

int main(int argc, char *argv[]) {
  add_fd(inet_open("localhost", atoi(argv[1])), read_data);
  add_fd(0, read_data);

  while(1) sleep(1);
}
