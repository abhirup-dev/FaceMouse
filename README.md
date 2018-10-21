# FaceMouse
A <b>hands-free</b> mouse input and navigation interface powered by facial expressions.  

[GIF](FaceMouse.gif)

<img src="https://github.com/codebuddha/FaceMouse/blob/master/FaceMouse.gif">

### Options ###
 - to Scroll : activate SCROLL_MODE and move up/down   
 - Left-Click : activate CLICK_MODE and blink with left eye
 - Right-Click : activate CLICK_MODE and blink with right eye
 - Blink with both eyes to activate CLICK_MODE
 - Open mouth to activate SCROLL_MODE (close to deactivate)
 - Move mouse pointer with motion of your head
 - Calibrate for personal use

### Software ###
 The source code is in C++11 \
 [Dlib](http://dlib.net/) for 68-point facial landmark detection. \
 [libX11](https://en.wikipedia.org/wiki/Xlib) and [XTEST](https://www.x.org/releases/X11R7.7/doc/libXtst/xtestlib.html) for controlling mouse-pointer. \
 [OpenCV (C++)](https://opencv.org/) for overlay Text, display and testing. 
