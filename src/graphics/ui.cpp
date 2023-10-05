#include "ui.hpp"
#include "debug.hpp"

namespace Ui {
  void spinner(
    SkCanvas *c,
    const SkColor color,
    const SkPoint center,
    const double radius,
    const double sizeVariance
  ) {
    static const double diameter = radius * 2;
    static const double PER = 0.9;
    double tnorm = fmod(1.0 * std::chrono::steady_clock::now().time_since_epoch().count() / 1000000000.0, PER) / PER;
    double scalnorm = (1.0 + cos(tnorm * 2.0 * M_PI)) / 2;
    SkRect rect = SkRect::MakeXYWH(
      center.fX - (diameter + sizeVariance * scalnorm) / 2,
      center.fY - (diameter + sizeVariance * scalnorm) / 2,
      diameter + sizeVariance * scalnorm,
      diameter + sizeVariance * scalnorm);

    SkPaint p;
    p.setAntiAlias(false);
    p.setColor(color);
    p.setStyle(SkPaint::kStroke_Style);
    p.setStrokeWidth(3 + scalnorm * 3);

    double cosPart = (-cos(tnorm * M_PI) + 1.0) / 2.0 * 4.0;
    // This is still buggy, but it works with this specific weighted average
    double deg = 360 * (tnorm * 0.5 + cosPart * 0.5);
    c->drawArc(rect, deg, 90, false, p);
    c->drawArc(rect, deg + 180, 90, false, p);
  }
}