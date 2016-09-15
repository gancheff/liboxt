// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_progress.h - progress bar widget header file

class oxt_progress : public oxt_widget {
    int p;
  public:
    oxt_progress(int, int);
    void set_progress(int);
    void expose();
    void destroy();
};
