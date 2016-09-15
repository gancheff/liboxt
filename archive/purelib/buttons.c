#include "purelib.h"

void bpress(struct plWidget_t *);

int main(int argc, char *argv[]) {
  int i, j, x = 1, y = 1, n = 0;
  plWidget *window, *button[77];
  char name[77][10];

  plCreate(argc, argv);

  window = plWindowCreate(0, 0, 515, 310, "Button TEST", NORMAL);

  for (j=0; j<11; j++) {
    for (i=0; i<7; i++) {
      sprintf(name[n], "Button %d", n);
      button[n] = plButtonCreate(x, y, 70, 25, name[n], window);
//      plAddCallback(button[n], bpress);
      n++;
      x += 73;
    }

    x = 1;
    y += 28;
  }

  plEventLoop();

  return 0;
}

void bpress(struct plWidget_t *Button) {
  printf("%s pressed!\n", Button->buffer);
}
