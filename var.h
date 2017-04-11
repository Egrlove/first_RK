#ifndef var
#define var

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

Display *dpy;

void create_gc(Display *dpy, int screen, Window win,
  XFontStruct *fontInfo, GC *gc);
void config(XEvent *, geom, Window[], Window *);
void draw_resolution(Window win, Window indicator, geom getted_geom, GC gc,
                     int indicator_height, int indicator_width);



#endif
