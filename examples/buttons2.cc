#include "oxt.h"

int main(int argc, char *argv[]) {
  oxt_libinit(argc, argv);

  oxt_window window(NORMAL, "OXT Buttons Example");
  oxt_button *button;

  for (int j=0, x=5, y=5; j < 11; j++, x=5, y+= 25)
    for (int i=0; i < 7; i++, x+= 50) {
      button = new oxt_button("Button");
      button->set_pos(x, y);
      window.put(*button);
      button->appear();
    }

  window.set_size(380, 290);
  window.appear();

  oxt_run();

  return 0;
}
