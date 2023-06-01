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
  Window *window = new Window(800, 600);

  if (cfg.isSerialEnabled()) {
    serial = new Serial("/dev/ttyUSB0");
  }

  LOG("Window created");

  Input input(*serial);
  Output output(*serial, *window);

  // Context stack
  State::Context *stack = new State::Context();
  stack->state = new State::Initial(*stack);

  LOG("Initialization complete");

  auto lastClock = std::chrono::steady_clock::now();
  auto curClock = lastClock;

  LOG("Beginning main loop");

  while (stack != nullptr) {
    lastClock = curClock;
    curClock = std::chrono::steady_clock::now();
    double delta = (curClock - lastClock).count() / 1000000000.0;

    // Tick main things
    window->tick(delta);

    State::Action action = { State::ActionType::NOP, nullptr };
    if (serial != nullptr) {
      input.tick();
      serial->tick(delta) ;
      stack->state->tick(input.getResult(), delta, action);
    } else {
      stack->state->tick(window->getInputState(), delta, action);
    }

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
  }

  LOG("Freeing hardware objects");

  delete serial;
  delete window;

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