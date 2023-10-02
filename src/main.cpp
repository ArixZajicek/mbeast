#include <iostream>
#include <chrono>
#include <cstring>
#include "config.hpp"
#include "debug.hpp"
#include "states.hpp"
#include "hardware.hpp"

int run(const Config &cfg) {
  LOG("Initializing objects");

  // Hardware devices
  Serial *serial = nullptr;
  Window *window = new Window(Visor::WIDTH * 6, Visor::HEIGHT * 6);

  if (cfg.isSerialEnabled()) {
    serial = new Serial(cfg.getSerialDevice());
  }

  double targetFrametime = 0;
  if (cfg.getFPS() > 0) {
    targetFrametime = 1.0 / cfg.getFPS();
  }

  LOG("Window created");

  Input input(*serial);
  Output output(*serial, *window);

  // Context stack
  State::StateContext *ctxStack = new State::StateContext();
  ctxStack->state = new State::Initial(*ctxStack);

  // We'll only use a single output state here
  OutputState outState = {
    visor: (Color *)malloc(Visor::WIDTH * Visor::HEIGHT * sizeof(Color)),
    ears: {0, 0, 0, 0},
  };

  LOG("Initialization complete");

  auto frameEnd = std::chrono::steady_clock::now();
  auto frameStart = frameEnd;

  LOG("Beginning main loop");

  while (ctxStack != nullptr) {
    double lastFrameDelta = (frameEnd - frameStart).count() / 1000000000.0;
    frameStart = std::chrono::steady_clock::now();

    // Tick main things
    window->tick(lastFrameDelta);

    State::StateContext *nextCtx = ctxStack;
    if (serial != nullptr) {
      input.tick();
      serial->tick(lastFrameDelta) ;
      ctxStack->state->tick(input.getResult(), lastFrameDelta, nextCtx);
    } else {
      ctxStack->state->tick(window->getInputState(), lastFrameDelta, nextCtx);
    }
    ctxStack->state->draw(outState);

    SDL_Surface *tempSurface = SDL_CreateRGBSurfaceFrom(
      outState.visor,
      Visor::WIDTH, Visor::HEIGHT,
      24,
      3 * Visor::WIDTH,
      0xFF, 0xFF00, 0xFF0000, 0x00
    );

    SDL_Rect targetRect = {0, 0, Visor::WIDTH * 6, Visor::HEIGHT * 6};

    SDL_BlitScaled(tempSurface, nullptr, window->getBackBuffer(), &targetRect);

    SDL_FreeSurface(tempSurface);
    window->flip();

    State::StateContext *temp = nullptr;
    if (nextCtx != ctxStack) {
      if (nextCtx->parent == ctxStack) {
        // ENTER NEW CONTEXT
        LOG("Entering new context");
        ctxStack = nextCtx;
        ctxStack->state->enter(false);
      } else if (ctxStack->parent == nextCtx) {
        // EXIT CURRENT CONTEXT
        LOG("Exiting current context");
        ctxStack->state->exit();
        delete ctxStack->state;
        delete ctxStack;
        ctxStack = nextCtx;
        ctxStack->state->enter(true);
      } else {
         ABORT("Invalid next context returned. Unable to continue.");
      }
    }

    double actualFrameTime = (std::chrono::steady_clock::now() - frameStart).count() / 1000000000.0;
    if (actualFrameTime < targetFrametime) {
      SDL_Delay((targetFrametime - (std::chrono::steady_clock::now() - frameStart).count() / 1000000000.0) * 1000);
    }
    frameEnd = std::chrono::steady_clock::now();
  }

  LOG("Freeing hardware objects");

  delete serial;
  delete window;
  delete outState.visor;

  LOG("Exiting loop");
  return 0;
}

int main(int argc, char **argv) {

  LOG("Starting mbeast");

  Config cfg(argc, argv);

  int argInit = cfg.getInitSuccess();
  if (argInit == 1) {
    return 0;
  } else if (argInit != 0) {
    return argInit;
  }

  LOG("Args parsed");

  run(cfg);

  LOG("Safely quitting application");
  return 0;
}