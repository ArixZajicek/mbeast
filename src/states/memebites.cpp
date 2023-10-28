#include <iostream>
#include <memory.h>
#include <chrono>
#include <string>
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
  MemeBites::MemeBites(StateContext &ctx) : IState(ctx) {
    // Process initial data
  }

  void MemeBites::enter(bool resume) {
    if (!resume) textProgress = 0;
  }

  void MemeBites::tick(const InputState &input, double d, StateContext *&next) {
    //std::cout << "Ticking initial with time delta " << d << std::endl;

    if (input.keysTyped[InputKey::BACK])
      next = ctx.parent;
    if (textProgress < 1)
      textProgress += d / 4;

  }

  void MemeBites::draw(OutputState &out) {
    out.cvs->clear(SK_ColorBLUE);

    const char *text[] = {
      "An error has occurred. To continue:",
      "Boop snout to return to normal operation, or",
      "press LPAW+RPAW to restart your mantled beast.",
      "You will lose any unsaved information.",
      "Error: 0E : 016F : E621FA00",
    };

    const int rows = 5;
    const int cols = 47;



    for (int row = 0; row < rows && row < textProgress * rows; row++) {

      if (row > rows * textProgress - 1) {
        char temp[cols] = "";
        strncat(temp, text[row], std::min(static_cast<int>(floor((rows * textProgress - row) * cols)), static_cast<int>(strlen(text[row]))));
        Ui::text(out.cvs, temp, { 0, 10.0f * row }, 10, Ui::Justify::LEFT, SK_ColorWHITE);
      } else {
        Ui::text(out.cvs, text[row], { 0, 10.0f * row }, 10, Ui::Justify::LEFT, SK_ColorWHITE);
      }
    }
  }

  void MemeBites::exit() {

  }
}