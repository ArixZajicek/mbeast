#include <iostream>
#include <chrono>
#include <cstring>
#include "state/states.hpp"
#include "hardware/hardware.hpp"

void loop();
void init();
void cleanup();

// Context stack
State::Context *stack = nullptr;

int main(int argc, char **argv) {
  init();
  loop();
  cleanup();
}

void loop() {
  auto lastClock = std::chrono::steady_clock::now();
  auto curClock = lastClock;

  Serial serial("/dev/ttyUSB0");

  Input input(&serial);
  Output output(&serial);

  while (stack != nullptr) {

    lastClock = curClock;
    curClock = std::chrono::steady_clock::now();
    double delta = (curClock - lastClock).count() / 1000000000.0;

    input.tick();
    serial.tick(delta);

    State::Action action = { State::ActionType::NOP, nullptr };
    stack->state->tick(input.getResult(), delta, action);

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
}

void init() {
  // Initialize SDL, hub75 lib, devices, etc.
}

void cleanup() {}
