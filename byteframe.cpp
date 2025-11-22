#include <cstdint>
#include <vector>
#include <cmath>

#include <iostream>
// #include <stddef.h>
// #include <stdint.h>
// #include <stdlib.h>
// #include <stdbool.h>
// #include <unistd.h>
// #include <termios.h>
// #include <sys/select.h>

namespace bf {
  
struct Vec2 {
  float x, y; // might want to make it class later on
};

class Color {
public:
  uint8_t r, g, b, a;

  Color() : r(0), g(0), b(0), a(255) {}

  Color(uint8_t rr, uint8_t gg, uint8_t bb, uint8_t aa = 255) : r(rr), g(gg), b(bb), a(aa) {}

  uint32_t toU32()
  {
    return ( (uint32_t)a << 24) |
           ( (uint32_t)r << 16) |
           ( (uint32_t)g << 8)  |
           ( (uint32_t)b );
  }

  static Color toColor(uint32_t rgba)
  {
    Color c;
    c.a = (rgba >> 24) & 0xFF;
    c.r = (rgba >> 16) & 0xFF;
    c.g = (rgba >> 8)  & 0xFF;
    c.b = (rgba)       & 0xFF;
    return c;
  }
  
};


static Color GREY(0x18, 0x18, 0x18);
static Color RED(0xFF, 0x00, 0x00);
static Color WHITE(0xFF, 0xFF, 0xFF);
static Color GREEN(0x00, 0xFF, 0x00);
static Color BLUE(0x00, 0x00, 0xFF);

class Canvas {
public:
    Canvas(size_t w, size_t h)
        : width(w), height(h) {
        pixels.resize(w * h);
    }

  void fill(Color col){
    uint32_t rgba = col.toU32();
    for (size_t i=0; i<pixels.size(); ++i){
      pixels[i] = rgba;
    }
  }

  
  void drawRect(size_t w, size_t h, size_t x, size_t y, Color col){
    uint32_t rgba = col.toU32();

    // if (x > c->width || y > c->height) return; TODO: handle real out of bounds
   
    size_t x_end = (x + w > width)  ? width:  x + w;
    size_t y_end = (y + h > height) ? height: y + h;

    for (size_t j=y; j<y_end; ++j){
      for (size_t i=x; i<x_end; ++i){
        pixels[j * width + i] = rgba;
      }  
    }
  }

  
  void drawPixel(size_t x, size_t y, Color color){
    if (x >= width || y >= height){
      return;
    }
    pixels[y * width + x] = color.toU32();
  }

  
  void rd_canvas_to_ppm(const char *filename){
    FILE *f = fopen(filename, "wb");
  
    fprintf(f, "P6\n%zu %zu\n255\n", width, height);

    for (size_t i=0; i<height*width; ++i){
      Color c = Color::toColor(pixels[i]);

      fwrite(&c.r, 1, 1, f);
      fwrite(&c.g, 1, 1, f);
      fwrite(&c.b, 1, 1, f);
    }
    fclose(f);
  }


  void drawEllipse(int cx, int cy, float rx, float ry, Color col) {
    int minx = cx - rx;
    int miny = cy - ry;
    int maxx = cx + rx;
    int maxy = cy + ry;

    if (minx < 0) minx = 0;
    if (miny < 0) miny = 0;
    if (maxx >= width)  maxx = width - 1;
    if (maxy >= height) maxy = height - 1;

    for (int y = miny; y <= maxy; y++) {
      for (int x = minx; x <= maxx; x++) {
        float dx = x - cx;
        float dy = y - cy;

        if ((dx*dx)/(rx*rx) + (dy*dy)/(ry*ry) <= 1.0f) {
            drawPixel(x, y, col);
        }
      }
    }
  }

  
  void drawCircle(int cx, int cy, float r, Color col){
    drawEllipse(cx, cy, r, r, col);
  }


  
  void drawVline(size_t start, size_t end, size_t x, size_t w, Color col){
    for (size_t y=start; y<end; ++y){
      for (size_t j=0; j<w; ++j){
        // printf("%zu %zu", x, y);
        drawPixel(x+j, y, col);
        }
      }
  }

  void drawHline(size_t start, size_t end, size_t y, size_t w, Color col){
    for (size_t x=start; x<end; ++x){
      for (size_t j=0; j<w; ++j){
        // printf("%zu %zu", x, y);
        drawPixel(x, y+j, col);
        }
      }
  }

  void drawTriangle(Vec2 v1, Vec2 v2, Vec2 v3, Color col){

    int xmin = ceil(v1.x);
    int xmax = floor(v3.x);

    Vec2 mr = {.x = (v1.x + v3.x) * 0.5f}; 

    for (int x = xmin; x<=xmax; ++x){
      float hr;

      if (x<=mr.x){
        hr = rd_solve_y(v1, v2, (float)x);
      } else {
        hr = rd_solve_y(v3, v2, (float)x);
      }

      float ymin = v1.y;
      float ymax = hr;

      for (int y=ceil(ymin); y<=floor(ymax); ++y){
        drawPixel(x, y, col);
      }
    }
  }
  
  uint32_t* data() { return pixels.data(); } 
private:
    size_t width, height;
    std::vector<uint32_t> pixels;

  float rd_solve_y(Vec2 a, Vec2 b, float x){
    float slope = (b.y - a.y) / (b.x - a.x);
    return a.y + slope * (x - a.x); 
  }
};

}
