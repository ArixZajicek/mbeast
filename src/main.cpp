#include <iostream>
#include <chrono>
#include <cstring>
#include "config.hpp"
#include "debug.hpp"
#include "states.hpp"
#include "hardware.hpp"


// Context stack
  State::Context *stack = nullptr;

  // Hardware devices
  Serial *serial = nullptr;
  Window *window = nullptr;

  Input *input = nullptr;
  Output *output = nullptr;

int init() {
  LOG("Initializing objects");

  window = new Window(800, 600);
  LOG("Window created");

  input = new Input(serial, window);
  output = new Output(serial, window);

  State::Context initialContext;

  stack = new State::Context();
  stack->state = new State::Initial(stack);

  LOG("Initialization complete");
  return 0;
}

int loop() {
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
      input->tick();
      serial->tick(delta) ;
      stack->state->tick(input->getResult(), delta, action);
    } else {
      stack->state->tick(window->getInputState(), delta, action);
    }

    switch (action.type) {
    case State::ActionType::ENTER:
      action.context->parent = stack;
      stack = action.context;
      stack->state->enter();
      break;
    case State::ActionType::EXIT:
      stack->state->exit();
      stack = stack->parent;
      break;
    case State::ActionType::SWAP:
      stack->state->exit();
      action.context->parent = stack;
      stack = action.context;
      stack->state->enter();
      break;
    }
  }

  LOG("Exiting loop");
  return 0;
}

int cleanup() {
  LOG("Deleting remaining objects"); 

  delete output;
  delete input;
  delete window;
  delete serial;

  LOG("Cleanup complete");
  
  return 0;
}

int main(int argc, char **argv) {

  LOG("Starting mbeast");

  int argInit = Cfg.init(argc, argv);
  if (argInit == 1) {
    return 0;
  } else if (argInit != 0) {
    return argInit;
  }

  LOG("Args parsed");

  init();
  loop();
  cleanup();

  LOG("Safely quitting application");
  return 0;
}