#include "oxt.h"

void move_up(oxt_widget *button, oxt_data label) {
  static_cast<oxt_widget *>(label)->set_pos(static_cast<oxt_widget *>(label)->getx(), static_cast<oxt_widget *>(label)->gety()-5);
}

void move_down(oxt_widget *button, oxt_data label) {
  static_cast<oxt_widget *>(label)->set_pos(static_cast<oxt_widget *>(label)->getx(), static_cast<oxt_widget *>(label)->gety()+5);
}

void move_left(oxt_widget *button, oxt_data label) {
  static_cast<oxt_widget *>(label)->set_pos(static_cast<oxt_widget *>(label)->getx()-5, static_cast<oxt_widget *>(label)->gety());
}

void move_right(oxt_widget *button, oxt_data label) {
  static_cast<oxt_widget *>(label)->set_pos(static_cast<oxt_widget *>(label)->getx()+5, static_cast<oxt_widget *>(label)->gety());
}



int main(int argc, char *argv[]) {
  oxt_libinit(argc, argv);

  oxt_window window(NORMAL, "Move bar");
  oxt_label target("Move ME!");

  oxt_button up("UP"), down("DOWN"), left("LEFT"), right("RIGHT");

  window.put(up);
  window.put(down);
  window.put(left);
  window.put(right);

  up.set_pos(20, 5);
  left.set_pos(5, up.geth()+5);
  right.set_pos(left.getw()+5, up.geth()+5);
  down.set_pos(20, up.geth()+left.geth()+5);

  up.callback(move_up, (oxt_data)&target);
  down.callback(move_down, (oxt_data)&target);
  left.callback(move_left, (oxt_data)&target);
  right.callback(move_right, (oxt_data)&target);

  up.appear();
  left.appear();
  right.appear();
  down.appear();

  target.appear();

  window.set_size(100, 75);
  window.appear();

  oxt_run();
}
