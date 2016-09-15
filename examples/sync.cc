#include <cstring>
#include "oxt.h"
#include "oxt_util.h"

void read_data(int fd) {
  char buffer[1024];

  memset(buffer, 0, 1024);
  read(fd, buffer, 1024);
  write(1, buffer, 1024);
}

int main(int argc, char *argv[]) {
  oxt_libinit(argc, argv);

  oxt_add_fd(0, read_data);
  oxt_add_fd(oxt_inet_open("127.0.0.1", 31331), read_data);
  oxt_add_fd(oxt_inet_open("localhost", 31332), read_data);

  oxt_window window(NORMAL, "sync in oxt");
  window.appear();

  oxt_run();

  return 0;
}
