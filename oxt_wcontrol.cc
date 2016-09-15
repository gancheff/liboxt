// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_wcontrol.cc - widget control

#include <map>
#include "oxt_widget.h"
#include "oxt_internal.h"
using namespace std;

static map<Window, oxt_widget *> w_list;

void register_widget(oxt_widget *widget) {
  if (!root) {
    root = widget;
    root->next = root;
    root->prev = root;
    curr = root;
  } else {
    curr->next = widget;
    widget->next = root;
    widget->prev = curr;
    root->prev = widget;
    curr = widget;
  }

#ifdef DEBUG
  cout << app_basename << ": registering widget: " << widget->get_label() << endl;
#endif
  w_list.insert(pair<Window, oxt_widget *>(widget->win, widget));
}

oxt_widget *get_widget(Window w) {
//  oxt_widget *temp;

//  for (temp = root; (temp != 0) && (temp->win != w);
//	temp = (temp->next == root) ? 0 : temp->next);

//  if (!temp)
//    cout << app_basename << ": no widget for window: " << w << endl;

  return (w_list.find(w))->second;
}

void delete_widget(oxt_widget *widget) {
  w_list.erase(widget->win);

  widget->prev->next = widget->next;
  widget->next->prev = widget->prev;
}
