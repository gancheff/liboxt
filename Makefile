# it's generally a good idea to edit this file
# some of the problems during compilation can be fixed here

CC = g++
AR = ar
CPPFLAGS = -Wall -fPIC -funroll-loops -I/usr/X11R6/include -I./
LDFLAGS = -shared -L/usr/X11R6/lib -lX11 -lXext -lXpm
OBJS =  oxt_widget.o  \
	oxt_main.o    \
	oxt_wcontrol.o \
	oxt_util.o	\
	oxt_window.o  \
	oxt_box.o     \
	oxt_label.o   \
	oxt_pixmap.o  \
	oxt_button.o \
	oxt_progress.o
PREFIX = /usr/home/shudder
TARGET = liboxt2

%.o: %.cc
	@echo "Compiling $(basename $<)..."
	$(CC) $(CPPFLAGS) -c $<

all: $(TARGET).so $(TARGET).a

$(TARGET).so: $(OBJS)
	@echo "Linking library..."
	$(CC) $(LDFLAGS) $(OBJS) -o $(TARGET).so

$(TARGET).a: $(OBJS)
	$(AR) rc $(TARGET).a $(OBJS)

install: $(TARGET).so $(TARGET).a
	@echo "Installing in $(PREFIX)"
	@./INSTALL $(TARGET) $(PREFIX)
#	@ldconfig

clean:
	@echo "Cleaning..."
	@rm -f $(OBJS) $(TARGET).* core
