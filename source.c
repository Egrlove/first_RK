#include "var.h"
//===========================================================
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

//===========================================================
void create_gc(Display *dpy, int screen, Window win,
                      XFontStruct *fontInfo, GC *gc) {
  XGCValues gr_values;
  gr_values.font = fontInfo->fid;
  gr_values.foreground = BlackPixel(dpy, screen);
  *gc = XCreateGC(dpy, win, GCFont + GCForeground + GCBackground, &gr_values);
}
//===========================================================

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
//===========================================================
