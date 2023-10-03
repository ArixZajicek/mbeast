#include <iostream>
#include <memory.h>
#include "states.hpp"
#include "debug.hpp"
#include "include/core/SkColor.h"

namespace State {
  Initial::Initial(StateContext &ctx) : IState(ctx) {
    // Process initial data
  }

  void Initial::enter(bool resume) {

  }

  void Initial::tick(const InputState &input, double d, StateContext *&next) {
    //std::cout << "Ticking initial with time delta " << d << std::endl;
    if (input.keysTyped[InputKey::BACK]) next = ctx.parent;
    if (input.keysTyped[InputKey::ACTION]) {
      next = new StateContext({&ctx, nullptr});
      Neutral *n = new Neutral(*next);
      next->state = n;
      LOG("Next state set to new Neutral instance");
    }
  }

  void Initial::draw(OutputState &out) {
    static uint8_t r = 255, g = 0, b = 0;
    static int i = 0;
    out.rawPix[i] = SkColorSetRGB(r, g, b);

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

  void Initial::exit() {
    
  }
}