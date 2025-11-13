Very Simplistic software rendering library written in C

Examples of how it can be used in terminal and with raylib as a window provider in the examples folder
- To build Examples (requires raylib and terminal example only works for posix, however removing custom controls allows it to work with windows)
```bash
  cc -o builder builder.c
  ./builder
```


The idea is to just use render.h and provide your custom things you need on top of that, which is shown in renderfull.h adding a couple of helpers (some require posix)
