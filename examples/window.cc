#include <iostream>
#include "oxt.h"

using namespace std;

void quit(oxt_widget *window, oxt_data data) {
  cout << "goodbye cruel world!\n";
  oxt_quit = 1;
}

int main(int argc, char *argv[]) {
  oxt_libinit(argc, argv);

  oxt_window window(NORMAL, "Window Example");
  oxt_window lone_window(AVOID_WM, "this won't show");

  window.set_size(200, 100);
  window.callback(quit, 0);

  lone_window.set_pos(400, 400);

  window.appear();
  lone_window.appear();

  oxt_run();

  return 0;
}
