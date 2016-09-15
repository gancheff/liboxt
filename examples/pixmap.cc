#include "oxt.h"

void quit(oxt_widget *window, oxt_data data) {
  oxt_quit = 1;
}

int main(int argc, char *argv[]) {
  oxt_libinit(argc, argv);

//  oxt_window window(NORMAL, "Pixmap Example");
  oxt_pixmap pixmap(argv[1]);


//  window.put(pixmap);
//  window.callback(quit, 0);

  pixmap.appear();
//  window.appear();

  oxt_run();

  return 0;
}
