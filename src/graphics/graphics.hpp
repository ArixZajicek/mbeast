
#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "hardware/hardware.hpp"
#include "SDL2/SDL.h"

class Window {
public:
  Window(int w, int h);
  void tick(double delta);
  const SDL_Surface *getBackBuffer();
  void draw(Color *pixels, int x, int y, int w, int h);
  void flip();
  Input::State getInputState();

private:
  SDL_Window *window;
  SDL_Surface *windowSurface;
  SDL_Surface *bufferSurface;
  Input::State *inputState;

  int width, height;

};

#endif