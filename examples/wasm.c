typedef unsigned long size_t;
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

static unsigned char heap[64 * 1024 * 1024];
static size_t heap_pos = 0;

void *my_malloc(size_t size) {
  if (heap_pos + size > sizeof(heap)) return 0;
  void *p = &heap[heap_pos];
  heap_pos += size;
  return p;
}

#define RD_NO_STDLIB
#define RD_IMPLEMENTATION

#undef RD_ALLOC
#define RD_ALLOC(size) my_malloc(size) 

#include "render.h"


__attribute__((export_name("make_canvas")))
rd_canvas* make_canvas(size_t w, size_t h) {
  rd_canvas* c = RD_ALLOC(sizeof(rd_canvas));
  rd_init_canvas(c, w, h);
  rd_fill_background(c, rd_red);
  return c;
}

__attribute__((export_name("draw_demo")))
void draw_demo(rd_canvas* c) {
  rd_draw_rect(c, 50, 50, 10, 10, rd_blue);

  mt_Vec2 t1 = {.x=100, .y=50};
  mt_Vec2 t2 = {.x=200, .y=100};
  mt_Vec2 t3 = {.x=300, .y=50};

  rd_draw_triangle(c, t1, t2, t3, rd_green);
}

__attribute__((export_name("canvas_pixels")))
uint32_t* canvas_pixels(rd_canvas* c) {
  return c->pixels;
}

__attribute__((export_name("canvas_width")))
size_t canvas_width(rd_canvas* c) {
  return c->width;
}

__attribute__((export_name("canvas_height")))
size_t canvas_height(rd_canvas* c) {
  return c->height;
}
