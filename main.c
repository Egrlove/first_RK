#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  Window root_return;
  int x_return, y_return;
  unsigned int width_return, height_return;
  unsigned int border_width_return;
  unsigned int depth_return;
} geom;

static void create_gc(Display *dpy, int screen, Window win,
                      XFontStruct *fontInfo, GC *gc) {
  XGCValues gr_values;
  gr_values.font = fontInfo->fid;
  gr_values.foreground = WhitePixel(dpy, screen);
  *gc = XCreateGC(dpy, win, GCFont + GCForeground + GCBackground, &gr_values);
}

int main(int argc, char *argv[]) {
  Window win;
  XSetWindowAttributes attr;
  unsigned long mask = 0;
  int done = 0;
  int disp_width;
  int disp_height;
  int win_width = 500;
  int win_height = 500;
  int screen;
  char string[30];
  Display *dpy;
  XEvent event;
  GC gc;
  XSizeHints hint;
  XFontStruct *fontInfo;
  geom getted_geom;

  dpy = XOpenDisplay(NULL); /* X display structure */
  screen = DefaultScreen(dpy);
  disp_width = DisplayWidth(dpy, screen);
  disp_height = DisplayHeight(dpy, screen);
  fontInfo = XLoadQueryFont(dpy, "fixed");


  attr.background_pixel = XBlackPixel(dpy, screen);

  win = XCreateWindow(dpy, DefaultRootWindow(dpy), disp_width / 2,
                      disp_height / 2, win_width, win_height, 1,
                      DefaultDepth(dpy, screen), InputOutput, CopyFromParent,
                      CWBackPixel, &attr);

  mask = KeyPressMask | ExposureMask | StructureNotifyMask;
  XSelectInput(dpy, win, mask);

  hint.flags = (PMinSize | PPosition | PMaxSize);
  hint.min_width = 150;
  hint.min_height = 75;
  hint.max_width = disp_width;
  hint.max_height = disp_height;
  XSetNormalHints(dpy, win, &hint);

  create_gc(dpy, screen, win, fontInfo, &gc);

  XMapWindow(dpy, win);

  while (1) {
    XNextEvent(dpy, &event);
    switch (event.type) {
    case ConfigureNotify:

      break;

    case Expose:
      XGetGeometry(dpy, win, &getted_geom.root_return, &getted_geom.x_return,
                   &getted_geom.y_return, &getted_geom.width_return,
                   &getted_geom.height_return, &getted_geom.border_width_return,
                   &getted_geom.depth_return);

      sprintf(string, "%d--%d", getted_geom.width_return, getted_geom.height_return);
      XDrawString(dpy, win, gc, getted_geom.width_return / 2,
                  getted_geom.height_return / 2, string, strlen(string));

      break;

    case KeyPress:
      done = event.xkey.keycode;
      return 1;
      break;
    default:
      break;
    } /* switch */
  }   /* while */

  XDestroyWindow(dpy, win);
  XCloseDisplay(dpy);
  return (done);
}
