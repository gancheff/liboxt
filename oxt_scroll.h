// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_scroll.h - scroll bar widget header file

class oxt_scroll : public oxt_widget {
    Window au, ad, al, ar, clipper, scroller;
  public:
    oxt_scroll(int, int);
    void put(oxt_widget &);
    void button_motion();
    void expose();
    void destroy();
};
