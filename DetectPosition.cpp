#include <iostream>
#include <X11/Xlib.h>
int main()
{
    Display *display = XOpenDisplay(0);
    int screen = DefaultScreen(display);
    int win_x, win_y, root_x, root_y = 0;
    unsigned int mask = 0;
    Window child_win, root_win;
    while(true)
    {
        XQueryPointer(display, XRootWindow(display, screen), &child_win, &root_win, &root_x, &root_y, &win_x, &win_y, &mask);
        std::cout<<win_x<<' '<<win_y<<'\n';
    }
}