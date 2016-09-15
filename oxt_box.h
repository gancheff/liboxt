// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_box.h - box widget header file

#define VER_BOX 1
#define HOR_BOX 2

class oxt_box : public oxt_widget {
  private:
    int spc, loc;
  public:
    oxt_box(int, int, int);
    void put(oxt_widget &);
    void destroy();
};
