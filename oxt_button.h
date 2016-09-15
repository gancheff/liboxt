// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_button.h - button widget header file

#define NORMAL_BUTTON 0
#define FLAT_BUTTON 1
#define TOGGLE_BUTTON 2

class oxt_button : public oxt_widget {
    int pressed, inside;
    void draw_button(int, int);
  public:
    oxt_button(char *);
    void expose();
    void button_press();
    void button_release();
    void enter();
    void leave();
    void destroy();
};
