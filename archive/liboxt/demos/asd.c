#include <stdio.h>
#include "oxt_main.h"

void manage(oxt_Widget *);

int main(int argc, char *argv[]) {
  oxt_Widget *window, *button1, *button2;

  oxt_Init(argc, argv);
  window = oxt_CreateWindow(320, 240, 320, 240, "Demo Window", AVOIDWM);
  button1 = oxt_CreateButton(10, 10, 40, 20, "Hello!", window);
  button2 = oxt_CreateButton(60, 10, 40, 20, "Hi!", window);

  oxt_AddCallback(button1, manage);
  oxt_AddCallback(button2, manage);

  oxt_Show(window);
  oxt_Show(button1);
  oxt_Show(button2);

  oxt_EventLoop();

  return 0;
}

void manage(oxt_Widget *button) {
  printf("%s\n", button->buffer);
}
