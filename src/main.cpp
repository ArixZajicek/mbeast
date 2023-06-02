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
  State::Context *stack = new State::Context();
  stack->state = new State::Initial(*stack);

  // We'll only use a single output state here
  OutputState outState = {
    visor: (Color *)malloc(Visor::WIDTH * Visor::HEIGHT * sizeof(Color)),
    ears: {0, 0, 0, 0},
  };

  LOG("Initialization complete");

  auto frameEnd = std::chrono::steady_clock::now();
  auto frameStart = frameEnd;

  LOG("Beginning main loop");

  while (stack != nullptr) {
    double lastFrameDelta = (frameEnd - frameStart).count() / 1000000000.0;
    frameStart = std::chrono::steady_clock::now();

    // Tick main things
    window->tick(lastFrameDelta);

    State::Action action = { State::ActionType::NOP, nullptr };
    if (serial != nullptr) {
      input.tick();
      serial->tick(lastFrameDelta) ;
      stack->state->tick(input.getResult(), lastFrameDelta, action);
    } else {
      stack->state->tick(window->getInputState(), lastFrameDelta, action);
    }
    stack->state->draw(outState);

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

    State::Context *temp = nullptr;
    switch (action.type) {
    case State::ActionType::ENTER:
      action.context->parent = stack;
      stack = action.context;
      stack->state->enter();
      break;
    case State::ActionType::EXIT:
      stack->state->exit();
      temp = stack;
      stack = temp->parent;
      delete temp->state;
      delete temp;
      break;
    case State::ActionType::SWAP:
      stack->state->exit();
      action.context->parent = stack;
      stack = action.context;
      stack->state->enter();
      break;
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