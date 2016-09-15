/*	oxt_tree.h -- tree include file
	vladimir ganchev */

oxt_Widget *_index;
oxt_Widget *_current;
oxt_Widget *_active;

void _delete_widget(struct oxt_Widget *);
void _register_widget(struct oxt_Widget *);
oxt_Widget *_find_widget(Window);
