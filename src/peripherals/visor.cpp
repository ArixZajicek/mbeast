#ifdef HEADLESS

#include <iostream>

#include "led-matrix-c.h"

#include "main.hpp"
#include "peripherals.hpp"


struct RGBLedMatrix *matrix = NULL;
struct LedCanvas *backCanvas, *frontCanvas;

Visor::Visor() {
  LOG("starting with dimensions %dx%d", WIDTH, HEIGHT);

  struct RGBLedMatrixOptions matrix_opts = {
      NULL,   // name of hardware mapping
      64,     // --led-rows
      64,     // --led-cols
      2,      // --led-chain
      2,      // --led-parallel

      11,      // N/A --led-pwm-bits (default is 11)
      0,      // N/A --led-pwm-lsb-nanoseconds
      0,      // N/A --led-pwm-dither-bits

      100,     // --led-brightness

      0,      // N/A --led-scan-mode

      3,      // --led-row-addr-type

      0,      // N/A multiplexing
      0,      // N/A --led-hardware-pulse
      0,      // N/A --led-show-refresh
      0,      // N/A --led-inverse
      "BGR",  // --led-rgb-sequence
      NULL,   // N/A --led-pixel-mapper
      NULL,   // N/A --led-panel-type
      90       // N/A --led-limit-refresh
  };

  struct RGBLedRuntimeOptions matrix_runtime_opts = {
      5,      // --led-slowdown-gpio (used to be 4)
      0,      // N/A --led-daemon
      (bool)0,      // N/A do_gpio_init
      NULL,
      NULL
  };

  /*
  struct RGBLedMatrixOptions matrix_opts;
  matrix_opts.rows = 64;
  matrix_opts.cols = 64;
  matrix_opts.chain_length = 2;
  matrix_opts.parallel = 2;
  matrix_opts.brightness = 50;
  matrix_opts.row_address_type = 3;
  matrix_opts.led_rgb_sequence = "BGR";

  struct RGBLedRuntimeOptions matrix_runtime_opts;
  matrix_runtime_opts.gpio_slowdown = 5;
  */


  matrix = led_matrix_create_from_options_and_rt_options(&matrix_opts, &matrix_runtime_opts);
  frontCanvas = led_matrix_get_canvas(matrix);
  backCanvas = led_matrix_create_offscreen_canvas(matrix);
  led_canvas_fill(backCanvas, 0, 0xFF, 0);
  led_canvas_fill(frontCanvas, 0xFF, 0xFF, 0xFF);
}

/* Draw a full WIDTHxHEIGHT image provided here */
void Visor::draw(SkColor *image) {
  for (int i = 0; i < WIDTH * HEIGHT; ++i) {
    int x = i % WIDTH;
    int y = i / WIDTH + (x >= WIDTH / 2 ? HEIGHT : 0);

    led_canvas_set_pixel(backCanvas, x % (WIDTH / 2), y, SkColorGetR(*image), SkColorGetG(*image), SkColorGetB(*image));
    ++image;
  }
  LOG("copied new data into back canvas using manual fill");
}

void Visor::flip() {
  backCanvas = led_matrix_swap_on_vsync(matrix, backCanvas);
  LOG("buffer flip complete");
}

Visor::~Visor() {
  led_matrix_delete(matrix);
}

#endif
