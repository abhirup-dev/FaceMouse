# FaceMouse
A <b>hands-free</b> mouse input and navigation interface powered by facial expressions.  

[GIF](FaceMouse.gif)

<img src="https://github.com/codebuddha/FaceMouse/blob/master/FaceMouse.gif">

NB: The frames are horizontally flipped. 

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
 [OpenCV (C++)](https://opencv.org/) for overlay Text, markers, display and testing. 

 ### Tips ###
 - Using <b>X11</b>: open [sample_cursor.cpp](sample_cursor.cpp) and compile using `g++ sample_cursor.cpp -lX11`. Enter numbers between 1 to 5 to emulate different mouse keypresses.
 - If you mistakenly activate CLICK_MODE (stray blinks ¯\\\_(ツ)\_/¯), activate SCROLL_MODE momentarily to deactivate the former.
 - Set <b>frame sensitivies</b> for all actions. 
   ```
    LOW_FRAME_SENSITIVITY = 3,
	HIGH_FRAME_SENSITIVITY = 5, ...
    ```These the denote the number of frames the expression needs to be sustained for activation. This protects against stray blinks. Adjust it!
 - Avoid delaying the left/right blink action once CLICK_MODE is activated. Best if immediate!  


