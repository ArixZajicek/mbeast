#include "graphics.hpp"
#include "include/core/SkPath.h"
#include "include/core/SkRRect.h"

#define MAP(x1, y1, x2, y2, s) x1 + (x2 - x1) * s, y1 + (y2 - y1) * s

namespace Face {
  void drawEye(
    SkCanvas *c,
    SkPaint paintOutline,
    SkPaint paintPupil,
    float lookH,
    float lookV,
    float topEyelid,
    float bottomEyelid,
    float backCorner,
    float frontCorner
  ) {


    SkPath outer;

    // Start from bottom back corner
    outer.moveTo(4, 25);

    // Bottom eyelid
    outer.quadTo(MAP((4 + 38) / 2, (25 + 20) / 2, 21, 30, bottomEyelid), 38, 20);

    // Front corner
    outer.quadTo(MAP(38, 20, 32, 12, frontCorner), MAP(38, 20, 30, 4, frontCorner));

    // Top eyelid
    outer.quadTo(MAP((4 + 38) / 2, (25 + 20) / 2, 20, -4, topEyelid), MAP(4.2, 25, 4, 22, backCorner));

    // Back corner
    outer.quadTo(MAP(4.0, 25.0, 3.7, 23.0, backCorner), 4.0, 25.0);

    outer.close();

    c->drawPath(outer, paintOutline);

    c->save();
    c->clipPath(outer);

    c->drawOval(SkRect::MakeXYWH(17 + 11 * lookH, 10 - 6 * lookV - (2 * lookH), 12, 17), paintPupil);

    c->restore();
  }

  void moveForEyeSaving(SkCanvas *c, EyeIndex e) {
    c->save();
    if (e == EyeIndex::LBack || e == EyeIndex::LFront) {
      c->scale(-1, 1);
      c->translate(-256, 0);
    }

    if (e == EyeIndex::LFront || e == EyeIndex::RFront) {
      c->translate(40, 4);
      c->scale(0.9, 0.9);
    }
  }

  void drawMouth(SkCanvas *c, SkPaint outline, float open, float corner) {
    SkPath main;
    int Y_OFFSET = -8;
    main.moveTo(128, 55 + Y_OFFSET);
    main.lineTo(106, 46 + Y_OFFSET);
    main.lineTo(87, 56 - corner * 4 + Y_OFFSET);
    main.quadTo(71, 52, MAP(59, 46 - 4, 62, 38 - 4, corner));


    c->save();
    bool cont = true;
    for (int i = 0; i < 2; i++) {
      c->drawPath(main, outline);

      c->scale(-1, 1);
      c->translate(-256, 0);
    }
    c->restore();
  }
}