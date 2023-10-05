#include <chrono>
#include "include/core/SkCanvas.h"

#ifndef UI_HPP
#define UI_HPP

namespace Ui {
  void spinner(
    SkCanvas *c,
    const SkColor color,
    const SkPoint center = { 128, 32 },
    const double radius = 14,
    const double sizeVariance = 8
  );
}

#endif