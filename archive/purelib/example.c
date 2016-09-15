#include "purelib.h"

int clicks = 0;

void *fquit(struct plWidget_t *);
void *increment(struct plWidget_t *);
void *clear(struct plWidget_t *);

char buff1[1024], buff2[1024];

int main(int argc, char *argv[]) {
  plWidget *window, *button1, *edit, *edit1,
           *button2, *label, *bquit;

  memset(buff1, 0, 1024);
  memset(buff2, 0, 1024);

  plCreate(argc, argv);

  window = plWindowCreate(0, 0, 150, 200,
			"purelib app",
			NORMAL);
  button1 = plButtonCreate(-1, 10, 80, 25,
			"Click me!",
			window);
  button2 = plButtonCreate(-1, 40, 80, 25,
			"Clear",
			window);
  label = plLabelCreate(-1, 70, 150, 20,
			"0 clicks!",
			window);
  bquit = plButtonCreate(-1, 100, 80, 25,
			"Quit",
			window);
  edit = plEditCreate(5, 140, 140,
			buff1,
			window);
  edit1 = plEditCreate(5, 163, 140,
			buff2,
			window);

  plAddCallback(button1, increment);
  plAddCallback(bquit, fquit);
  plAddCallback(button2, clear);

  plLink(button1, label);
  plLink(button2, label);
  plLink(label, button1);

  plEventLoop();

  return 0;
}

void *fquit(struct plWidget_t *Button) {
  printf("Total clicks: %d\n", clicks);
  printf("Edit box 1: %s\nEdit box 2: %s\n", buff1, buff2);
  exit(0);
}

void *increment(struct plWidget_t *Button) {
  char *text = malloc(1024);

  sprintf(text, "%d clicks!", ++clicks);
  Button->link->buffer = text;

  Button->link->plExpose(Button->link);
}

void *clear(struct plWidget_t *Button) {
  clicks = 0;
  Button->link->buffer = "0 clicks!";
  Button->link->plExpose(Button->link);
}
