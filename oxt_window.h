// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_window.h - window widget header file

#define NORMAL 1
#define AVOID_WM 2

class oxt_window : public oxt_widget {
  public:
    oxt_window(int, char *);
    void close();
    void cmessage();
    void destroy();
    void put(oxt_widget &);
};
