// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_widget.h - widget header file

#include <X11/Xlib.h>

#define RAISED 1
#define SUNKEN 2
#define SINGLE 1
#define DOUBLE 2
#define GET_TYPE      3

class oxt_widget {
  protected:
    int x, y, w, h, s, type, len, t;
    char *label;
    void draw_frame(int, int, int, int);
  public:
    oxt_widget();
    Window win, parent;
    GC gc;
    oxt_widget *next, *prev;
    void (*callback_f) (oxt_widget *, void *), *data;

    void callback(void (*)(oxt_widget *, void *), void *);
    void set_size(int, int);
    void set_pos(int, int);
    void set_label(char *);
    void toggle();
    void appear();
    void hide();

    int get_status();
    int set_type(int);
    int is_toggle();
    int getx();
    int gety();
    int getw();
    int geth();

    char *get_label();

    virtual void set_active(int a) { }
    virtual void close() { }
    virtual void expose() { }
    virtual void cmessage() { }
    virtual void button_press() { }
    virtual void button_release() { }
    virtual void button_motion() { }
    virtual void enter() { }
    virtual void leave() { }
    virtual void destroy() { }
    virtual void put() { }
};
