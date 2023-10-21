#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "include/core/SkCanvas.h"

namespace Face {
  enum EyeIndex { RBack = 0, RFront, LBack, LFront };

  /** */
  void drawEye(
    SkCanvas *c,
    SkPaint paintOutline,
    SkPaint paintPupil,
    float lookH = 0,
    float lookV = 0,
    float topEyelid = 1,
    float bottomEyelid = 1,
    float backCorner = 1,
    float frontCorner = 1
  );

  void moveForEyeSaving(SkCanvas *c, EyeIndex e);

  void drawMouth(SkCanvas *c, SkPaint outline, float open, float corner);
}

namespace Ui {
  enum Justify {
    LEFT, CENTER, RIGHT
  };

  double period(double d);

  void spinner(
    SkCanvas *c,
    const SkColor color,
    const SkPoint center = { 128, 32 },
    const double radius = 14,
    const double sizeVariance = 8
  );

  void text(
    SkCanvas *c,
    const char text[],
    const SkPoint origin,
    const double size,
    const Justify j,
    const SkColor color
  );
}

#endif