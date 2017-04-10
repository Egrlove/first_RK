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
  gr_values.foreground = BlackPixel(dpy, screen);
  *gc = XCreateGC(dpy, win, GCFont + GCForeground + GCBackground, &gr_values);
}

void config(XEvent *, geom, Window[], Window *);
void draw_resolution(Window win, Window indicator, geom getted_geom, GC gc,
                     int indicator_height, int indicator_width);

Display *dpy;

int main(int argc, char *argv[]) {
  Window win;

  //=============================
  Window indicator;
  int indicator_width = 100;
  int indicator_height = 100;
  int panel_width = 180;
  int panel_height = 140;
  Window panel;
  Window plus_box;
  Window minus_box;
  Window V_box;
  Window tools[4];
  //=============================
  XSetWindowAttributes attr;
  unsigned long mask = 0;
  int done = 0;
  int disp_width;
  int disp_height;
  int win_width = 500;
  int win_height = 500;
  int screen;

  XEvent event;
  GC gc;
  int depth;
  XSizeHints hint;
  XFontStruct *fontInfo;
  geom getted_geom;

  dpy = XOpenDisplay(NULL); /* X display structure */
  screen = DefaultScreen(dpy);
  disp_width = DisplayWidth(dpy, screen);
  disp_height = DisplayHeight(dpy, screen);
  depth = DefaultDepth(dpy, screen);
  fontInfo = XLoadQueryFont(dpy, "fixed");

  attr.background_pixel = 0x00FF00;

  /* Main Window */
  win = XCreateWindow(dpy, DefaultRootWindow(dpy), disp_width / 2,
                      disp_height / 2, win_width, win_height, 1, depth,
                      InputOutput, CopyFromParent, CWBackPixel, &attr);

  mask = ButtonPressMask | ButtonReleaseMask | KeyPressMask | ExposureMask |
         StructureNotifyMask | CWEventMask;
  XSelectInput(dpy, win, mask);

  hint.flags = (PMinSize | PPosition | PMaxSize);
  hint.min_width = indicator_width;
  hint.min_height = indicator_height;
  hint.max_width = disp_width;
  hint.max_height = disp_height;
  XSetNormalHints(dpy, win, &hint);
  //=======

  /* indicator window  */
  attr.override_redirect = True;
  attr.background_pixel = 0x00FFFF; /* WhitePixel(dpy, src); */
  attr.event_mask = (ButtonPressMask | ExposureMask);

  indicator =
      XCreateWindow(dpy, win, win_width / 2, win_height / 2, indicator_width,
                    indicator_height, 0, depth, InputOutput, CopyFromParent,
                    (CWOverrideRedirect | CWBackPixel | CWEventMask), &attr);
  //=============
  /* Tools panel */
  attr.override_redirect = True;
  attr.background_pixel = 0x666666; /* WhitePixel(dpy, src); */
  attr.event_mask =
      (ButtonPressMask | ButtonReleaseMask | KeyPressMask | ExposureMask);

  panel = XCreateWindow(
      dpy, win, 0, 0, panel_width, panel_height / 4, 0, depth, InputOutput,
      CopyFromParent, (CWOverrideRedirect | CWBackPixel | CWEventMask), &attr);
  //=============
  /* Tools  for  panel */
  attr.override_redirect = True;
  attr.background_pixel = WhitePixel(dpy, screen); /* WhitePixel(dpy, src); */
  attr.event_mask =
      (ButtonPressMask | ButtonReleaseMask | KeyPressMask | ExposureMask);
  tools[0] = 0;
  tools[1] = plus_box =
      XCreateWindow(dpy, panel, 5, 5, panel_width / 5, panel_height / 5, 0,
                    depth, InputOutput, CopyFromParent,
                    (CWOverrideRedirect | CWBackPixel | CWEventMask), &attr);

  tools[2] = minus_box =
      XCreateWindow(dpy, panel, 5 + panel_width / 4, 5, panel_width / 5,
                    panel_height / 5, 0, depth, InputOutput, CopyFromParent,
                    (CWOverrideRedirect | CWBackPixel | CWEventMask), &attr);

  tools[3] = V_box =
      XCreateWindow(dpy, panel, 5 + panel_width / 2, 5, panel_width / 5 * 2,
                    panel_height / 5, 0, depth, InputOutput, CopyFromParent,
                    (CWOverrideRedirect | CWBackPixel | CWEventMask), &attr);

  //=============
  XSelectInput(dpy, panel, mask);
  XSelectInput(dpy, tools[1], mask);
  XSelectInput(dpy, tools[2], mask);
  XSelectInput(dpy, tools[3], mask);
  create_gc(dpy, screen, win, fontInfo, &gc);

  XMapWindow(dpy, win);
  XMapSubwindows(dpy, win);
  XMapSubwindows(dpy, panel);

  while (1) {
    XNextEvent(dpy, &event);
    switch (event.type) {
    case ConfigureNotify:
      break;
    case Expose:
      XDrawString(dpy, plus_box, gc, 17, 20, "+", sizeof(char));
      XDrawString(dpy, minus_box, gc, 17, 20, "-", sizeof(char));
      XDrawString(dpy, V_box, gc, 30, 20, "V/H", 3 * sizeof(char));
      draw_resolution(win, indicator, getted_geom, gc, indicator_height,
                      indicator_width);
      break;

    case ButtonPress:
      config(&event, getted_geom, tools, &win);
      if (event.xbutton.window == indicator)
        return (1);
      break;

    default:
      break;
    } /* switch */
  }   /* while */

  XDestroyWindow(dpy, win);
  XCloseDisplay(dpy);
  return (done);
}

void config(XEvent *ev, geom getted_geom, Window tools[], Window *win) {
  int delta = 20;
  int i;
  static int mode = 0; /* mode=0 - V ; mode=1 -H  */
  for (i = 0; i < 4; ++i) {
    if (ev->xbutton.window == tools[i])
      break;
  }
  XGetGeometry(dpy, *win, &getted_geom.root_return, &getted_geom.x_return,
               &getted_geom.y_return, &getted_geom.width_return,
               &getted_geom.height_return, &getted_geom.border_width_return,
               &getted_geom.depth_return);
  switch (i) {
  case 1:
    mode ? XResizeWindow(dpy, *win, getted_geom.width_return + delta,
                         getted_geom.height_return)
         : XResizeWindow(dpy, *win, getted_geom.width_return,
                         getted_geom.height_return + delta);
    break;

  case 2:
    mode ? XResizeWindow(dpy, *win, getted_geom.width_return - delta,
                         getted_geom.height_return)
         : XResizeWindow(dpy, *win, getted_geom.width_return,
                         getted_geom.height_return - delta);
    break;
  case 3:
    mode = mode ? 0 : 1;
    break;
  default:
    break;
  }
}

void draw_resolution(Window win, Window indicator, geom getted_geom, GC gc,
                     int indicator_height, int indicator_width) {
  char string[30];

  XGetGeometry(dpy, win, &getted_geom.root_return, &getted_geom.x_return,
               &getted_geom.y_return, &getted_geom.width_return,
               &getted_geom.height_return, &getted_geom.border_width_return,
               &getted_geom.depth_return);

  sprintf(string, "%d--%d", getted_geom.width_return,
          getted_geom.height_return);
  XClearWindow(dpy, indicator);
  XMoveWindow(dpy, indicator,
              getted_geom.width_return / 2 - indicator_width / 2,
              getted_geom.height_return / 2 - indicator_height / 2);

  XGetGeometry(dpy, indicator, &getted_geom.root_return, &getted_geom.x_return,
               &getted_geom.y_return, &getted_geom.width_return,
               &getted_geom.height_return, &getted_geom.border_width_return,
               &getted_geom.depth_return);

  XDrawString(dpy, indicator, gc, indicator_width / 4, indicator_height / 2,
              string, strlen(string));
}
