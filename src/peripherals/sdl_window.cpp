#ifndef HEADLESS

#include "main.hpp"
#include "peripherals.hpp"
#include "SDL2/SDL.h"

SDL_Window *window;
SDL_Surface *windowSurface;
SDL_Joystick *gameController = nullptr;
const int WINDOW_SCALAR = 5;

Visor::Visor() {
  LOG("starting SDL window with resolution %dx%d", WIDTH, HEIGHT);

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
    ABORT("SDL could not initialize!\n%s", SDL_GetError());
  }

  window = SDL_CreateWindow(
    "Mantled Beast Debug Window",
    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    WIDTH * WINDOW_SCALAR, HEIGHT * WINDOW_SCALAR,
    SDL_WINDOW_SHOWN
  );

  if (window == nullptr) {
    ABORT("SDL could not create a window!\n%s", SDL_GetError());
  }

  windowSurface = SDL_GetWindowSurface(window);
  SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0xFF, 0xFF, 0xFF));
  SDL_UpdateWindowSurface(window);

  LOG("SDL Window created");

  if (SDL_NumJoysticks() < 1) {
    LOG("No joysticks found. Not initializing.");
  } else {
    gameController = SDL_JoystickOpen(0);
    if (gameController == nullptr) {
      LOG("Could not initialize joystick 0: %s", SDL_GetError());
    }
  }
}

void Visor::draw(SkColor *image) {
  SDL_Surface *temp = SDL_CreateRGBSurfaceFrom(
    image,
    WIDTH, HEIGHT,
    32,
    4 * WIDTH,
    0xFF0000, 0xFF00, 0xFF, 0x00
  );

  SDL_Rect rect = {
    0, 0, WIDTH * WINDOW_SCALAR, HEIGHT * WINDOW_SCALAR
  };

  SDL_BlitScaled(temp, NULL, windowSurface, &rect);
  SDL_FreeSurface(temp);
}

void Visor::flip() {
  SDL_UpdateWindowSurface(window);
}

Visor::~Visor() {
  LOG("Freeing window surface");

  SDL_FreeSurface(windowSurface);
  SDL_DestroyWindow(window);
  SDL_JoystickClose(gameController);
  SDL_Quit();

  LOG("SDL Services freed");
}

Input::Input(Serial *s) {}

inline void keyact(InputState &state, InputKey k, bool pressed) {
  if (pressed) {
    if (state.keysDown[k] != true) state.keysTyped[k] = true;
    state.keysDown[k] = true;
  } else {
    state.keysDown[k] = false;
  }
}

void Input::tick() {
  // New tick, reset typed keys
  for (int k = 0; k < InputKey::__COUNT; k++) {
    state.keysTyped[k] = false;
  }

  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
    case SDL_KEYDOWN: case SDL_KEYUP:
      // LOG("SDL KEY EVENT FOUND: %d", e.key.keysym.sym);
      switch (e.key.keysym.sym) {
      case SDLK_UP:
        keyact(state, InputKey::UP, e.type == SDL_KEYDOWN);
        break;
      case SDLK_DOWN:
        keyact(state, InputKey::DOWN, e.type == SDL_KEYDOWN);
        break;
      case SDLK_LEFT:
        keyact(state, InputKey::LEFT, e.type == SDL_KEYDOWN);
        break;
      case SDLK_RIGHT:
        keyact(state, InputKey::RIGHT, e.type == SDL_KEYDOWN);
        break;
      case SDLK_x:
        keyact(state, InputKey::ACTION, e.type == SDL_KEYDOWN);
        break;
      case SDLK_z:
        keyact(state, InputKey::BACK, e.type == SDL_KEYDOWN);
        break;
      case SDLK_SPACE:
        keyact(state, InputKey::BOOP, e.type == SDL_KEYDOWN);
        break;
      }
      break;
    case SDL_JOYBUTTONDOWN:
      LOG("Joy button %d pressed", e.jbutton.which);
      break;
    case SDL_QUIT:
      keyact(state, InputKey::BACK, true);
      break;
    }
  }
}

InputState Input::getResult() {
  return state;
}

Input::~Input() {
  LOG("Exiting input object");
}

#endif