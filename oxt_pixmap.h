// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_pixmap.h - pixmap widget header file

class oxt_pixmap : public oxt_widget {
    Pixmap pixmap, shape;
  public:
    oxt_pixmap(char *);
    oxt_pixmap(char **);
    void expose();
    void destroy();
};
