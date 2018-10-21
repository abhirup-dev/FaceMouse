#include <X11/Xlib.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

void mouseClick(int button)
{
	Display *display = XOpenDisplay(NULL);

	XEvent event;

	if(display == NULL)
	{
		fprintf(stderr, "Errore nell'apertura del Display !!!\n");
		exit(EXIT_FAILURE);
	}

	memset(&event, 0x00, sizeof(event));

	event.type = ButtonPress;
	event.xbutton.button = button;
	event.xbutton.same_screen = True;

	XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

	event.xbutton.subwindow = event.xbutton.window;

	while(event.xbutton.subwindow)
	{
		event.xbutton.window = event.xbutton.subwindow;

		XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
	}

	if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");

	XFlush(display);

	usleep(100000);

	event.type = ButtonRelease;
	event.xbutton.state = 0x100;

	if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");

	XFlush(display);

	XCloseDisplay(display);
}

int main(int argc, char * argv[]) {
	int i=0;
    int x, y;
    int win_x, win_y, root_x, root_y = 0;
	unsigned int mask = 0;
    Display *display = XOpenDisplay(0);
    Window root = DefaultRootWindow(display);
    Window child_win, root_win;`1
	while(true)
	{
		XQueryPointer(display, root, &child_win, &root_win, &root_x, &root_y, &win_x, &win_y, &mask);
		std::cout<< win_x << " " << win_y <<"\n";
		std::cin >> x;
		if(x==0)continue;
		auto b = x==1?Button1: x==2?Button2: x==3?Button3: x==4?Button4: Button5;
		XWarpPointer(display, None, root, 0, 0, 0, 0, win_x, win_y);
		mouseClick(b);
		XFlush(display);
	}
	XCloseDisplay(display);
    return 0;
}
