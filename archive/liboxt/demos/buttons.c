#include "oxt_main.h"

int main (int argc, char *argv[]) {
  int i, j, x = 1, y = 1, n = 0;
  oxt_Widget *window, *button[77];
  char name[77][10];

  oxt_Init (argc, argv);

  window = oxt_CreateWindow (0, 0, 515, 310, "Button TEST", NORMAL);

  for (j = 0; j < 11; j++) {
      for (i = 0; i < 7; i++) {
	  sprintf (name[n], "Button %d", n);
	  button[n] = oxt_CreateButton (x, y, 70, 25, name[n], window);
	  oxt_Show (button[n]);
	  n++;
	  x += 73;
	}

      x = 1;
      y += 28;
    }

  oxt_Show(window);
  oxt_EventLoop();

  return 0;
}
