#include "oxt_main.h"
#include "oxt_internal.h"

void oxt_calculate(oxt_Widget *button);
void oxt_quit(oxt_Widget *button);

char eb1[100], eb2[100];
oxt_Widget *window, *e1, *e2, *b1, *b2;

int main(int argc, char *argv[]) {
  memset(eb1, 0, 100);
  memset(eb2, 0, 100);

  oxt_Init(argc, argv);

  window = oxt_CreateWindow(0, 0, 180, 130, "Calculator", NORMAL);
  oxt_Show(window);

  e1 = oxt_CreateEdit(10, 10, 140, eb1, window);
  oxt_Show(e1);

  e2 = oxt_CreateEdit(10, 35, 140, eb2, window);
  oxt_Show(e2);

  b1 = oxt_CreateButton(10, 70, 80, 20, "Calculate", window);
  oxt_Show(b1);

  b2 = oxt_CreateButton(100, 70, 50, 20, "Quit", window);
  oxt_Show(b2);

  oxt_AddCallback(b1, oxt_calculate);
  oxt_AddCallback(b2, oxt_quit);

  oxt_EventLoop();
  return 0;
}

void oxt_calculate(oxt_Widget *button) {
  int a = atoi(eb1),
      b = atoi(eb2);
  memset(eb1, 0, 100);
  memset(eb2, 0, 100);
  sprintf(eb1, "Result: %d", a+b);
  _frame(e1, bcolor.white, SUNKEN, -1, -1);
  _frame(e2, bcolor.white, SUNKEN, -1, -1);
  e1->expose(e1);
  e2->expose(e2);
}

void oxt_quit(oxt_Widget *button) {
  printf("Quitting...\n");
  oxt_Quit();
}
