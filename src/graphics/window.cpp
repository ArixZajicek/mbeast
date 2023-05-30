#include "graphics.hpp"
#include "common.hpp"

#include <iostream>

Window::Window(int w, int h) {
  LOG("starting SDL window with resolution %dx%d", w, h);
  this->width = w;
  this->height = h;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    ABORT("SDL could not initialize!" << std::endl << SDL_GetError());
  }

  window = SDL_CreateWindow(
    "HUB75 Matrix Debug Window",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    w, h,
    SDL_WINDOW_SHOWN
  );

  if (window == nullptr) {
    ABORT("SDL could not create a window!" << std::endl << SDL_GetError());
  }

  windowSurface = SDL_GetWindowSurface(window);
  SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0xFF, 0xFF, 0xFF));
  SDL_UpdateWindowSurface(window);
}

inline void keyact(Input::State *state, Input::Key k, bool pressed) {
  if (pressed) {
    if (state->keysDown[k] == false) state->keysTyped[k] = true;
    state->keysDown[k] = true;
  } else {
    state->keysDown[k] = false;
  }
}

void Window::tick(double delta) {
  // New tick, reset typed keys
  for (int k = 0; k < Input::Key::__COUNT; k++) {
    inputState->keysTyped[k] = false;
  }

  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
    case SDL_KEYDOWN: case SDL_KEYUP:
      switch (e.key.keysym.sym) {
      case SDLK_UP:
        keyact(inputState, Input::Key::UP, e.type == SDL_KEYDOWN);
        break;
      case SDLK_DOWN:
        keyact(inputState, Input::Key::DOWN, e.type == SDL_KEYDOWN);
        break;
      case SDLK_LEFT:
        keyact(inputState, Input::Key::LEFT, e.type == SDL_KEYDOWN);
        break;
      case SDLK_RIGHT:
        keyact(inputState, Input::Key::RIGHT, e.type == SDL_KEYDOWN);
        break;
      case SDLK_x:
        keyact(inputState, Input::Key::ACTION, e.type == SDL_KEYDOWN);
        break;
      case SDLK_z:
        keyact(inputState, Input::Key::BACK, e.type == SDL_KEYDOWN);
        break;
      case SDLK_SPACE:
        keyact(inputState, Input::Key::BOOP, e.type == SDL_KEYDOWN);
        break;
      }
      break;
    case SDL_QUIT:
      // do something probably but idk what yet
      break;
    }
  }
}

/* For more involved draw operations */
const SDL_Surface *Window::getBackBuffer() {
  return windowSurface;
}

/* For simple draw operations, converting from our native color format */
void Window::draw(Color *pixels, int x, int y, int w, int h) {
  SDL_Surface *temp = SDL_CreateRGBSurfaceFrom(
    pixels,
    width, height,
    24,
    3 * width,
    0xFF, 0xFF00, 0xFF0000, 0x00
  );

  SDL_Rect rect = {
    x, y, w, h
  };

  SDL_BlitSurface(temp, NULL, bufferSurface, &rect);
}

void Window::flip() {
  SDL_BlitSurface(bufferSurface, nullptr, windowSurface, nullptr);
  SDL_UpdateWindowSurface(window);
}

Input::State Window::getInputState() {
  return *inputState;
}

Window::~Window() {
  SDL_FreeSurface(windowSurface);
  windowSurface = nullptr;
  SDL_DestroyWindow(window);
  window = nullptr;
  SDL_Quit();
  SDL_DestroyWindow(window);
  SDL_VideoQuit();
}