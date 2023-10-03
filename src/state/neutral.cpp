#include <iostream>
#include <memory.h>
#include <cmath>
#include "states.hpp"
#include "include/core/SkCanvas.h"

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
    static float deg = 0;

    SkRect rect = SkRect::MakeXYWH(112, 16, 32, 32);

    SkPaint p;
    p.setAntiAlias(false);
    p.setColor(SK_ColorWHITE);
    p.setStyle(SkPaint::kStrokeAndFill_Style);
    p.setStrokeWidth(2);
    out.cvs->drawRect(rect, p);

    p.setColor(SK_ColorRED);
    p.setStyle(SkPaint::kStroke_Style);
    out.cvs->drawArc(rect, deg, 90, false, p);
    //out.cvs->drawLine(8, 8, 56, 56, p);


    deg = deg + (360 / 60);
    if (deg >= 360) deg -= 360;
  }

  void Neutral::exit() {
    
  }
}