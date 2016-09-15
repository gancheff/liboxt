CC = gcc
LD = ld
AR = ar
STRIP = strip
CFLAGS = -g -Wall -fPIC -funroll-loops -I/usr/X11R6/include -I./
LDFLAGS = -shared -L/usr/X11R6/lib -lX11
OBJS = oxt_main.o oxt_internal.o oxt_tree.o oxt_timer.o oxt_window.o oxt_label.o oxt_button.o oxt_edit.o oxt_checkbox.o
SOLIB = liboxt.so
ARLIB = liboxt.a
LIBDIR = /usr/home/shudder/lib
TARGETS = $(SOLIB) $(ARLIB)

all: $(TARGETS)
install: $(TARGETS)
	cp $(TARGETS) $(LIBDIR)

$(SOLIB): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(SOLIB)
#	$(STRIP) --strip-all $(SOLIB)
#	$(STRIP) --remove-section=".comment" $(SOLIB)
#	$(STRIP) --remove-section=".note" $(SOLIB)

$(ARLIB): $(OBJS)
	$(AR) rs $(ARLIB) $(OBJS)

clean:
	rm -f $(OBJS) $(TARGETS) core
