#include <iostream>
#include <memory.h>
#include <chrono>
#include "include/core/SkColor.h"
#include "include/core/SkFont.h"
#include "include/core/SkTypeface.h"
#include "include/core/SkTextBlob.h"
#include "include/core/SkMaskFilter.h"
#include "include/core/SkFontMgr.h"
#include "main.hpp"
#include "states.hpp"
#include "graphics.hpp"

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
    }
  }

  void Initial::draw(OutputState &out) {
    out.cvs->clear(SK_ColorBLACK);
    Ui::spinner(out.cvs, SK_ColorWHITE, { 128, 26 });

    char text[20] = "Loading";
    strncat(text, "...", floor(Ui::period(0.75) * 4));
    Ui::text(out.cvs, text, { 128, 50 }, 8, Ui::Justify::CENTER, SK_ColorCYAN);
  }

  void Initial::exit() {

  }
}