#include <iostream>
#include <memory.h>
#include "states.hpp"

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
    static uint8_t r = 255, g = 0, b = 0;
    static int i = 0;
    out.visor[i] = {255, 255, 255};

    if (r == 255 && b == 0 && g < 255) {
      g++;
    } else if (g == 255 && r > 0) {
      r--;
    } else if (g == 255 && b < 255) {
      b++;
    } else if (b == 255 && g > 0) {
      g--;
    } else if (b == 255 && r < 255) {
      r++;
    } else if (r == 255 && b > 0) {
      b--;
    }

    i = i + 1 % (Visor::WIDTH * Visor::HEIGHT);
  }

  void Neutral::exit() {
    
  }
}