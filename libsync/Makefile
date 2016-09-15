CC = gcc
LD = ld
STRIP = strip
CFLAGS = -g -Wall -fPIC
LDFLAGS = -shared
TARGET = libsync.so
PREFIX = /usr/local

all: $(TARGET)

$(TARGET): libsync.c
	$(CC) $(CFLAGS) -c libsync.c
	$(LD) $(LDFLAGS) -o $(TARGET) libsync.o

stripped: $(TARGET)
	$(STRIP) -R .comment -R .note $(TARGET)

install: $(TARGET)
	cp $(TARGET) $(PREFIX)/lib/
	cp libsync.h $(PREFIX)/include/
	ldconfig

e: libsync.c
	$(CC) -o example example.c libsync.c

clean:
	rm -f *.o core $(TARGET) example
