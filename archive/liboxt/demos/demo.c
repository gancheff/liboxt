#include "oxt_main.h"
#include "oxt_internal.h"
#include "oxt_tree.h"

int clicks = 0;

void fquit(oxt_Widget *);
void increment(oxt_Widget *);
void clear(oxt_Widget *);

char labeltext[1024], buff1[1024], buff2[1024], *checkbuffer[6]={"Black",
						"White",
						"Red",
						"Green",
						"Blue",
						"Yellow"};

oxt_Widget *check[6];

int main(int argc, char *argv[]) {
  int i, position;
  oxt_Widget *window, *button1, *edit, *edit1,
           *button2, *label, *bquit;

  memset(labeltext, 0, 1024);
  memset(buff1, 0, 1024);
  memset(buff2, 0, 1024);

  sprintf(labeltext, "%d clicks!", clicks);

  oxt_Init(argc, argv);

  window = oxt_CreateWindow(0, 0, 280, 200,
			"Open X Toolkit demo",
			NORMAL);
  oxt_Show(window);
  button1 = oxt_CreateButton(35, 10, 80, 25,
			"Click!",
			window);
  oxt_Show(button1);
  button2 = oxt_CreateButton(35, 40, 80, 25,
			"Clear",
			window);
  oxt_Show(button2);
  label = oxt_CreateLabel(20, 70, 110, 15,
			labeltext,
			window);
  oxt_Show(label);
  bquit = oxt_CreateButton(35, 100, 80, 25,
			"Quit",
			window);
  oxt_Show(bquit);
  edit = oxt_CreateEdit(5, 140, 140,
			buff1,
			window);
  oxt_Show(edit);
  edit1 = oxt_CreateEdit(5, 163, 140,
			buff2,
			window);
  oxt_Show(edit1);

  for (i=0, position=0; i<6; i++) {
    check[i] = oxt_CreateCheckbox(170, position+=20, checkbuffer[i], window);
    oxt_Show(check[i]);
  }

  oxt_AddCallback(button1, increment);
  oxt_AddCallback(button2, clear);
  oxt_AddCallback(bquit, fquit);

  oxt_Link(button1, label);
  oxt_Link(button2, label);

  oxt_EventLoop();

  return 0;
}

void fquit(oxt_Widget *Button) {
  int i;
  printf("Total clicks: %d\n", clicks);
  printf("Edit box 1: %s\nEdit box 2: %s\n", buff1, buff2);

  for (i=0; i<6; i++)
    if (check[i]->content) printf("%s:\t\t\033[1;32mselected\033[0m\n", checkbuffer[i]);
    else printf("%s:\t\t\033[1;31mnot selected\033[0m\n", checkbuffer[i]);
  oxt_Quit();
}

void increment(oxt_Widget *Button) {
  sprintf(labeltext, "%d clicks!", ++clicks);
  Button->link->expose(Button->link);
}

void clear(oxt_Widget *Button) {
  clicks = 0;
  sprintf(labeltext, "0 clicks!");
  Button->link->expose(Button->link);
}
