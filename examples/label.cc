#include "oxt.h"

void quit(oxt_widget *window, oxt_data data) {
  oxt_quit = 1;
}

int main(int argc, char *argv[]) {
  oxt_libinit(argc, argv);

  oxt_window window(NORMAL, "Label Example");
  window.callback(quit, 0);
  oxt_label label("Hello, oxt!");
  label.set_size(label.getw()+10, label.geth()+10);
  window.put(label);
  label.appear();
  window.appear();

  oxt_run();

  return 0;
}
