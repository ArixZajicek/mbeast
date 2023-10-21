#include <iostream>
#include <memory.h>
#include <cmath>

#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"

#include "main.hpp"
#include "states.hpp"
#include "graphics.hpp"


// Nice cyan
// #define CLR(i) SkColorSetRGB(20 * i, 220 * i, 255 * i)

// Amber
// #define CLR(i) SkColorSetRGB(255 * i, 132 * i, 0 * i)

// Green
#define CLR(i) SkColorSetRGB(64 * i, 240 * i, 64 * i)

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
    //auto blue = SkColorSetRGB(20, 220, 255);

    SkPaint pOut;
    //pOut.setStrokeWidth(2);
    pOut.setColor(CLR(1));
    pOut.setStyle(SkPaint::kFill_Style);

    SkPaint pPupil;
    //pPupil.setColor(SkColorSetRGB(255, 180, 30));
    pPupil.setColor(CLR(0.4));
    pPupil.setStyle(SkPaint::kFill_Style);


    SkPaint pLine;
    pLine.setStrokeWidth(3);
    pLine.setColor(CLR(1));
    pLine.setStyle(SkPaint::kStroke_Style);

    out.cvs->clear(CLR(0.1));
    out.cvs->save();



    double t3 = cos(Ui::period(3) * 2 * M_PI);
    double t5 = cos(Ui::period(5) * 2 * M_PI);

    double nonSinT14 = Ui::period(14);
    double t14 = (sin(nonSinT14 * 2 * M_PI) + 1) / 2;
    double t7 = (sin(fmod(nonSinT14, 0.5) * 4 * M_PI) + 1) / 2;

    for (int i = 0; i < 4; i++) {
      Face::moveForEyeSaving(out.cvs, static_cast<Face::EyeIndex>(i));
      if (t14 > 111) {
        out.cvs->drawLine(11, 6, 34, 26, pLine);
        out.cvs->drawLine(35, 2, 11, 29, pLine);
      } else {
        Face::drawEye(out.cvs, pOut, pPupil, t3, t5, t7 + t14 / 2, t7, t14 * 6, t7);
      }
      out.cvs->restore();
    }

    // Mouth?
    Face::drawMouth(out.cvs, pLine, 0, t14);
  }

  void Neutral::exit() {

  }
}