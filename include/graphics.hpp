#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SDL2/SDL.h>
#include "types.hpp"

class Window {
public:
  Window(int w, int h);
  void tick(double delta);
  const SDL_Surface *getBackBuffer();
  void draw(Color *pixels, int x, int y, int w, int h);
  void flip();
  InputState getInputState();
  ~Window();

private:
  SDL_Window *window;
  SDL_Surface *windowSurface;
  SDL_Surface *bufferSurface;
  InputState inputState;

  int width, height;
};

#endif