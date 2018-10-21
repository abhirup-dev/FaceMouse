#include <iostream>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>
Display* dpy=XOpenDisplay(0);
int scr=XDefaultScreen(dpy);
//Window root_window=XRootWindow(dpy, scr);
int main()
{
    int ch;
    do
    {
        std::cin>>ch;
        if(ch == 3)
        {
            XTestFakeButtonEvent(dpy, 3, True, 0);
			XFlush(dpy);
			XTestFakeButtonEvent(dpy, 3, False, 0);
			XFlush(dpy);
			usleep(50);
            std::cout<<"Right clicked!!!\n";
        }
        else if(ch == 2)
        {
            XTestFakeButtonEvent(dpy, 2, True, 0);
			XFlush(dpy);
			XTestFakeButtonEvent(dpy, 2, False, 0);
			XFlush(dpy);
			usleep(50);
            std::cout<<"Left clicked!!!\n";
        }
        else if(ch == 4)
        {
            XTestFakeButtonEvent(dpy, 4, True, 0);
			XFlush(dpy);
			XTestFakeButtonEvent(dpy, 4, False, 0);
			XFlush(dpy);
			usleep(50);
            std::cout<<4 << " clicked!!!\n";
        }
        else if(ch == 5)
        {
            XTestFakeButtonEvent(dpy, 5, True, 0);
			XFlush(dpy);
			XTestFakeButtonEvent(dpy, 5, False, 0);
			XFlush(dpy);
			usleep(50);
            std::cout<<5 << " clicked!!!\n";
        }
    }while(true);
}
