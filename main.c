#include "var.h"

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
  XSetWindowAttributes attr;
  //=============================
  unsigned long mask = 0;
  int done = 0;
  int disp_width;
  int disp_height;
  int win_width = 500;
  int win_height = 500;
  char c;
  int screen;
  int depth;

  XEvent event;
  GC gc;
  XSizeHints hint;
  XFontStruct *fontInfo;
  geom getted_geom;

  if (argc < 2) {
    printf("Net Arguments\n");
    return 1;
  }
  sscanf(argv[1], "%d%c%d", &win_width, &c, &win_height);

  dpy = XOpenDisplay(NULL); /* X display structure */
  screen = DefaultScreen(dpy);
  disp_width = DisplayWidth(dpy, screen);
  disp_height = DisplayHeight(dpy, screen);
  depth = DefaultDepth(dpy, screen);
  fontInfo = XLoadQueryFont(dpy, "fixed");
  attr.background_pixel = 0x00FF00;

  if (win_width > disp_width || win_height > disp_height) {
    printf("Too high resolution, used default\n");
    win_width = 500;
    win_height = 500;
  }

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
