#include <iostream>
#include "led-matrix-c.h"

#include "common.hpp"
#include "hardware.hpp"

Visor::Visor() {
  LOG("starting with dimensions %dx%d", WIDTH, HEIGHT);

  // Initialize double buffers
  backBuffer = (Color *)malloc(WIDTH * HEIGHT * sizeof(Color));
  frontBuffer = (Color *)malloc(WIDTH * HEIGHT * sizeof(Color));
  LOG("finished allocating buffers");

  struct RGBLedMatrixOptions matrix_opts = {
      NULL,   // name of hardware mapping
      64,     // --led-rows
      64,     // --led-cols
      2,      // --led-chain
      2,      // --led-parallel

      2,      // N/A --led-pwm-bits (default is 11)
      0,      // N/A --led-pwm-lsb-nanoseconds
      0,      // N/A --led-pwm-dither-bits

      50,     // --led-brightness

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

  matrix = led_matrix_create_from_options_and_rt_options(&matrix_opts, &matrix_runtime_opts);
  frontCanvas = led_matrix_get_canvas(matrix);
  backCanvas = led_matrix_create_offscreen_canvas(matrix);
  led_canvas_fill(backCanvas, 0, 0xFF, 0);
  led_canvas_fill(frontCanvas, 0xFF, 0xFF, 0xFF);
}

/* Draw a full WIDTHxHEIGHT image provided here */
void Visor::draw(Color *image) {
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      led_canvas_set_pixel(backCanvas, x % WIDTH / 2, y + HEIGHT * (x / (WIDTH / 2)), image->r, image->g, image->b);
    }
  }
  LOG("copied new data into back canvas using manual fill");
}

void Visor::flip() {
  backCanvas = led_matrix_swap_on_vsync(matrix, backCanvas);
  LOG("buffer flip complete");
}

Visor::~Visor() {
  led_matrix_delete(matrix);

  if (backBuffer != nullptr || frontBuffer != nullptr) {
    free(backBuffer);
    free(frontBuffer);
    backBuffer = nullptr;
    frontBuffer = nullptr;
  }
}
