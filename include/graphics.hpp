#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "include/core/SkCanvas.h"
#include "types.hpp"

#ifndef HEADLESS
  #include <SDL2/SDL.h>
  class Window {
  public:
    Window(int w, int h);
    ~Window();

    void tick(double delta);
    InputState getInputState();
    void flip();

    SDL_Surface *getBackBuffer() const;
    void draw(Color *pixels, int x, int y, int w, int h) const;

  private:
    SDL_Window *window;
    SDL_Surface *windowSurface;
    SDL_Surface *bufferSurface;
    InputState inputState;

    int width, height;
  };
#endif

namespace Face {
  enum EyeIndex { RBack = 0, RFront, LBack, LFront };

  /** */
  void drawEye(
    SkCanvas *c,
    SkPaint paintOutline,
    SkPaint paintPupil,
    float lookH = 0,
    float lookV = 0,
    float topEyelid = 1,
    float bottomEyelid = 1,
    float backCorner = 1,
    float frontCorner = 1
  );

  void moveForEyeSaving(SkCanvas *c, EyeIndex e);

  void drawMouth(SkCanvas *c, SkPaint outline, float open, float corner);
}

#endif