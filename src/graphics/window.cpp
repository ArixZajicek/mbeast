#include "graphics.hpp"
#include "debug.hpp"

#ifndef HEADLESS
Window::Window(int w, int h) {
  LOG("starting SDL window with resolution %dx%d", w, h);
  this->width = w;
  this->height = h;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    ABORT("SDL could not initialize!\n%s", SDL_GetError());
  }

  window = SDL_CreateWindow(
    "HUB75 Matrix Debug Window",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    w, h,
    SDL_WINDOW_SHOWN
  );

  if (window == nullptr) {
    ABORT("SDL could not create a window!\n%s", SDL_GetError());
  }

  windowSurface = SDL_GetWindowSurface(window);
  SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0xFF, 0xFF, 0xFF));
  SDL_UpdateWindowSurface(window);

  LOG("SDL Window created");
}

inline void keyact(InputState &state, InputKey k, bool pressed) {
  if (pressed) {
    if (state.keysDown[k] == false) state.keysTyped[k] = true;
    state.keysDown[k] = true;
  } else {
    state.keysDown[k] = false;
  }
}

void Window::tick(double delta) {
  // New tick, reset typed keys
  for (int k = 0; k < InputKey::__COUNT; k++) {
    inputState.keysTyped[k] = false;
  }

  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
    case SDL_KEYDOWN: case SDL_KEYUP:
      LOG("SDL KEY EVENT FOUND");
      switch (e.key.keysym.sym) {
      case SDLK_UP:
        keyact(inputState, InputKey::UP, e.type == SDL_KEYDOWN);
        break;
      case SDLK_DOWN:
        keyact(inputState, InputKey::DOWN, e.type == SDL_KEYDOWN);
        break;
      case SDLK_LEFT:
        keyact(inputState, InputKey::LEFT, e.type == SDL_KEYDOWN);
        break;
      case SDLK_RIGHT:
        keyact(inputState, InputKey::RIGHT, e.type == SDL_KEYDOWN);
        break;
      case SDLK_x:
        keyact(inputState, InputKey::ACTION, e.type == SDL_KEYDOWN);
        break;
      case SDLK_z:
        keyact(inputState, InputKey::BACK, e.type == SDL_KEYDOWN);
        break;
      case SDLK_SPACE:
        keyact(inputState, InputKey::BOOP, e.type == SDL_KEYDOWN);
        break;
      }
      break;
    case SDL_QUIT:
      keyact(inputState, InputKey::BACK, true);
      break;
    }
  }
}

/* For more involved draw operations */
SDL_Surface *Window::getBackBuffer() const {
  return windowSurface;
}

/* For simple draw operations, converting from our native color format */
void Window::draw(Color *pixels, int x, int y, int w, int h) const {
  SDL_Surface *temp = SDL_CreateRGBSurfaceFrom(
    pixels,
    w, h,
    24,
    3 * w,
    0xFF, 0xFF00, 0xFF0000, 0x00
  );

  SDL_Rect rect = {
    x, y, w, h
  };

  SDL_BlitSurface(temp, NULL, windowSurface, &rect);
  SDL_FreeSurface(temp);
}

void Window::flip() {
  //SDL_BlitSurface(bufferSurface, nullptr, windowSurface, nullptr);
  SDL_UpdateWindowSurface(window);
}

InputState Window::getInputState() {
  return inputState;
}

Window::~Window() {
  LOG("Freeing window surface");

  //SDL_FreeSurface(bufferSurface);
  SDL_DestroyWindow(window);
  SDL_Quit();

  LOG("SDL Services freed");
}

#endif