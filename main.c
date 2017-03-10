#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>

#define WIDTH 320
#define HEIGHT 80


int main(int argc, char* argv[])
{
 Window win;
  XSetWindowAttributes attr;
  unsigned long mask =0;
  int done =0;
  int depth;
  int disp_width;
  int disp_height;
  int screen;
  XEvent event;
  GC gc;
  Pixmap pix;
  XSizeHints hint;

 Display* dpy = XOpenDisplay(NULL);              /* X display structure */
 Window root = DefaultRootWindow(dpy);
 screen =  DefaultScreen(dpy);
 gc = DefaultGC(dpy, screen);
 depth = DefaultDepth(dpy, screen);
 disp_width  = DisplayWidth(dpy, screen);
 disp_height = DisplayHeight(dpy,screen);
 pix = XCreatePixmap(dpy, root, WIDTH, HEIGHT, depth);


 XSetForeground(dpy, gc, BlackPixel(dpy, screen));
 XFillRectangle(dpy, pix, gc, 0, 0, WIDTH, HEIGHT);



 attr.override_redirect = False;
 attr.background_pixmap = pix;
 mask = (CWOverrideRedirect | CWBackPixmap);


 win = XCreateWindow(dpy, DefaultRootWindow(dpy), disp_width/2, disp_height/2, 200, 200, 1,   DefaultDepth(dpy, screen),
                    InputOutput, CopyFromParent, (CWOverrideRedirect | CWBackPixmap), &attr);

 mask = KeyPressMask;
 XSelectInput(dpy, win, mask);
 hint.flags = (PMinSize | PPosition | PMaxSize);
 hint.min_width = 150;
 hint.min_height = 75;     /* window min size */
 hint.max_width  = disp_width;
 hint.max_height = disp_height;   /* window max size */

 XSetNormalHints(dpy, win, &hint);

XMapRaised(dpy, win);
while(done == 0) {
  XNextEvent(dpy, &event);
  switch(event.type) {
    case KeyPress:      done = event.xkey.keycode;
                        break;
    default:            break;
  } /* switch */
} /* while */

XDestroyWindow(dpy, win);
XCloseDisplay(dpy);
return(done);


}
