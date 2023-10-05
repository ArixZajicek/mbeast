#include <iostream>
#include <memory.h>
#include <cmath>
#include "states.hpp"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"

namespace State {
  Neutral::Neutral(StateContext &ctx) : IState(ctx) {
    // Process Neutral data
  }

  void Neutral::enter(bool resume) {

  }

  void Neutral::tick(const InputState &input, double d, StateContext *&next) {
    //std::cout << "Ticking Neutral with time delta " << d << std::endl;
    if (input.keysTyped[InputKey::BACK]) next = ctx.parent;
  }

  void Neutral::draw(OutputState &out) {

  }

  void Neutral::exit() {

  }
}