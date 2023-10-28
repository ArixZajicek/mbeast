#include <iostream>
#include <memory.h>
#include <cmath>
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "main.hpp"
#include "states.hpp"
#include "graphics.hpp"

// Nice cyan
#define CLR(i) SkColorSetRGB(20 * i, 220 * i, 255 * i)
// Amber
// #define CLR(i) SkColorSetRGB(255 * i, 132 * i, 0 * i)
// Green
//#define CLR(i) SkColorSetRGB(64 * i, 240 * i, 64 * i)

namespace State {
  double randDouble(double min, double max) {
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX) * (max - min) + min;
  }

  double randDouble(double max) {
    return randDouble(0, max);
  }

  Neutral::Neutral(StateContext &ctx) : IState(ctx) {}

  void Neutral::enter(bool resume) {
    h = 0;
    v = 0;
    t_h = 0;
    t_v = 0;
    timeToNextBlink = randDouble(12);
    timeToNextShift = randDouble(9);
    blink = -1.0;
  }

  void Neutral::tick(const InputState &input, double d, StateContext *&next) {
    if (input.keysTyped[InputKey::BACK]) next = ctx.parent;
    else if (input.keysTyped[InputKey::ACTION]) {
      next = new StateContext({ &ctx, nullptr });
      MemeBites *n = new MemeBites(*next);
      next->state = n;
    } else {
      timeToNextBlink -= d;
      timeToNextShift -= d;

      if (timeToNextBlink < 0) {
        timeToNextBlink = randDouble(12);
        blink = 1.0;
      } else if (blink > -1.0) {
        blink -= d * 12;
        if (blink < -1.0) blink = -1.0;
      }

      if (timeToNextShift < 0) {
        timeToNextShift = randDouble(9);
        t_h = randDouble(-1, 1);
        t_v = randDouble(-1, 1);
      }

      if (std::abs(t_h - h) > 0.01) {
        h += (t_h - h) * 0.5 * d * 20;
      }
      if (std::abs(t_v - v) > 0.01) {
        v += (t_v - v) * 0.5 * d * 20;
      }
    }
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
        const double PEAK = 0.75;
        double hL = h < 0 ? PEAK + h * PEAK * 2 : PEAK;
        double hR = h > 0 ? PEAK - h * PEAK * 2 : PEAK;

        Face::drawEye(
          out.cvs,
          pOut,
          pPupil,
          (i >= 2 ? hL : hR),
          v,
          std::abs(blink * 1),
          0,
          0,
          std::abs(blink * 0.5)
        );
      }
      out.cvs->restore();
    }

    out.cvs->save();
    out.cvs->translate(0, -2 + t3 * 2);
    Face::drawMouth(out.cvs, pLine, 0, t3 * 0.1 + 0.1);
    out.cvs->restore();

    Face::drawNose(out.cvs, pLine);
  }

  void Neutral::exit() {

  }
}