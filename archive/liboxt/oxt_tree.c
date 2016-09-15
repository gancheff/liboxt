/*	oxt_tree.c -- tree control
	vladimir ganchev */

#include "oxt_main.h"
#include "oxt_internal.h"
#include "oxt_tree.h"

/*
void _register_widget(struct oxt_Widget *widget) {
  if (!_index) {
    _index = widget;
    _index->next = _index;
    _index->prev = _index;
    _current = _index;
  } else {
    _current->next = widget;
    widget->next = _index;
    widget->prev = _current;
    _index->prev = widget;
    _current = widget;
  }
}
*/
void _register_widget(struct oxt_Widget *widget) {
  printf("registering widget #%ld\n", widget->win);

  if (!base) {
    base = widget->win;
    *root = widget;
    widget->prev = widget;
    widget->next = widget;
  } else
    *(root+((widget->win)-base)) = widget;
    widget->next = root;
    widget->prev = *(root-((widget->win)-base));
}

void _delete_widget(struct oxt_Widget *widget) {
  widget->prev->next = widget->next;
  widget->next->prev = widget->prev;
}

oxt_Widget *_find_widget(Window win) {
  oxt_Widget *_temp = _index;

  while (_temp->win != win)
    _temp = _temp->next;

  return _temp;
}
