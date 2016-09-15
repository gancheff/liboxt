// oxt2 - open X toolkit
// (c) 2004 vladimir ganchev
//
// oxt_main.h - main header file

extern int oxt_quit;
typedef void * oxt_data;

void oxt_libinit(int, char **);
void oxt_run();
unsigned long oxt_color(int, int, int);
unsigned long oxt_color(char *);
