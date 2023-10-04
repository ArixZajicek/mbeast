#include <iostream>
#include <memory.h>
#include <chrono>
#include "states.hpp"
#include "debug.hpp"
#include "include/core/SkColor.h"
#include "ui.hpp"

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
      next = new StateContext({ &ctx, nullptr });
      Neutral *n = new Neutral(*next);
      next->state = n;
      LOG("Next state set to new Neutral instance");
    }
  }

  void Initial::draw(OutputState &out) {
    uint8_t r = 255, g = 0, b = 0;
    static int colorProgress = 0;

    for (int i = 0; i < Visor::WIDTH * Visor::HEIGHT; i++) {
      if ((i < colorProgress && colorProgress > 0) || (colorProgress < 0 && i > Visor::WIDTH * Visor::HEIGHT + colorProgress)) {
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
        out.rawPix[i] = SkColorSetRGB(r, g, b);
      } else {
        out.rawPix[i] = SK_ColorBLACK;
      }
    }

    colorProgress += 19;
    if (colorProgress >= Visor::WIDTH * Visor::HEIGHT) colorProgress -= 2 * Visor::WIDTH * Visor::HEIGHT;

    Ui::spinner(out.cvs, SK_ColorWHITE);
  }

  void Initial::exit() {

  }
}