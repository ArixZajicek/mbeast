#include <chrono>
#include "include/core/SkCanvas.h"

#ifndef UI_HPP
#define UI_HPP

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