#include <iostream>
#include "oxt.h"

using namespace std;

void quit(oxt_widget *window, oxt_data data) {
  oxt_quit = 1;
}

void press(oxt_widget *button, oxt_data data) {
  cout << "button press\n";
}

void toggle(oxt_widget *button, oxt_data data) {
  button->toggle();
  cout << "button toggle: " << button->is_toggle() << endl;

}

int main(int argc, char *argv[]) {
  oxt_libinit(argc, argv);

  oxt_window window(NORMAL, "Button Example");
  oxt_button *button;

  int j, x, y;

  for (j=0, x=2, y=2; j < 8; j++, x=2, y+= 25)
    for (int i=0; i < 5; i++, x+= 60) {
      switch(j) {
	case 0:
	case 1:
	  button = new oxt_button("Normal");
	  button->callback(press, 0);
	  break;
	case 2:
	case 3:
          button = new oxt_button("Flat");
          button->set_type(FLAT_BUTTON);
	  button->callback(press, 0);
	  break;
	case 4:
	case 5:
	  button = new oxt_button("Toggle");
	  button->callback(toggle, 0);
	  break;
	case 6:
	case 7:
	  button = new oxt_button("FToggle");
	  button->set_type(FLAT_BUTTON);
	  button->callback(toggle, 0);
      }

      button->set_pos(x, y);
      window.put(*button);
      button->appear();
    }

  window.set_size(300, 200);
  window.callback(quit, 0);
  window.appear();

  oxt_run();

  return 0;
}
