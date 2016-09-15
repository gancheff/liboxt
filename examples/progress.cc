#include "oxt.h"
#include "oxt_util.h"
#include <cstdlib>

int r = 0;
int s = 0;
oxt_progress *prg;

void change(int z) {
  if (!s) r = 0;
  else if (s == prg->getw()-5) r = 1;

  if (r) s--;
  else s++;

  prg->set_progress(s);
}

int main(int argc, char *argv[]) {
  oxt_libinit(argc, argv);

  oxt_window w(NORMAL, "progress");
  prg = new oxt_progress(100, 20);
  w.put(*prg);
  w.set_size(250, 50);
  prg->set_pos(10, 10);

  oxt_set_timer(0, 5, change);

  prg->appear();
  w.appear();

  oxt_run();
}
