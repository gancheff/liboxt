// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_label.h - label widget header file

class oxt_label : public oxt_widget {
  public:
    oxt_label(char *);
    void expose();
    void destroy();
};
