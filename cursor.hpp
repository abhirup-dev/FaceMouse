#include <X11/Xlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

void mouseClick(int x, Display *dpy)
{
	XTestFakeButtonEvent(dpy, x, True, 0);
				XFlush(dpy);
				XTestFakeButtonEvent(dpy, x, False, 0);
				XFlush(dpy);
//				usleep(50);
}
std::pair<int,int> getCurrentPos(Display *display, Window &root)
	{
	    int win_x, win_y, root_x, root_y = 0;
	    unsigned int mask = 0;
	    Window child_win, root_win;

		XQueryPointer(display, root, &child_win, &root_win, &root_x, &root_y, &win_x, &win_y, &mask);
		return std::make_pair(win_x, win_y);
	}
//int main(int argc, char * argv[]) {
//	int i=0;
//    int x, y;
//    int win_x, win_y, root_x, root_y = 0;
//	unsigned int mask = 0;
//    Display *display = XOpenDisplay(0);
//    Window root = DefaultRootWindow(display);
//    Window child_win, root_win;
//	while(true)
//	{
//		XQueryPointer(display, root, &child_win, &root_win, &root_x, &root_y, &win_x, &win_y, &mask);
//		std::cout<< win_x << " " << win_y <<"\n";
//		std::cin >> x;
//		if(!x)continue;
//		auto b = x==1?Button1: x==2?Button2: x==3?Button3: x==4?Button4: Button5;
//		XWarpPointer(display, None, root, 0, 0, 0, 0, win_x, win_y);
//		mouseClick(b);
//		XFlush(display);
//	}
//	XCloseDisplay(display);
//    return 0;
//}
