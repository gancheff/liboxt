#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "oxt_util.h"
using namespace std;

void timing(int timer) {
  cout << "timer " << timer << " activated!\n";
}

void killing(int fd) {
  int id, elapse;
  char buffer[40], cmd[10];

  memset(buffer, 0, 40);
  memset(cmd, 0, 10);

  read(fd, buffer, 40);
  sscanf(buffer, "%s %d %d", cmd, &id, &elapse);

  if (!strcmp(cmd, "kill")) oxt_kill_timer(id);
  if (!strcmp(cmd, "start")) oxt_set_timer(id, elapse, timing);
}

int main(void) {
  oxt_set_timer(0, 100, timing);

  oxt_add_fd(0, killing);
  oxt_add_fd(oxt_inet_open("localhost", 31331), killing);

  while(1) sleep(1);

  return 0;
}
