all:
	gcc -shared -fPIC -O2 -I/usr/X11R6/include -L/usr/X11R6/lib -o libpure.so purelib.c plWindow.c plLabel.c plButton.c plEdit.c -lX11

example:
	gcc -I./ -L../../lib -o example example.c -lpure
