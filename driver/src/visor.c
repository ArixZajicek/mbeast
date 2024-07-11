#include "led-matrix-c.h"
#include "mb.h"

struct RGBLedMatrix *matrix = NULL;
struct LedCanvas *backCanvas, *frontCanvas;

const int MBV_WIDTH = 256;
const int MBV_HEIGHT = 64;

void MBV_create() {
    LOG("starting with dimensions %dx%d", MBV_WIDTH, MBV_HEIGHT);

    struct RGBLedMatrixOptions matrix_opts = {
        NULL,   // name of hardware mapping
        64,     // --led-rows
        64,     // --led-cols
        2,      // --led-chain
        2,      // --led-parallel

        8,      // N/A --led-pwm-bits (default is 11)
        0,      // N/A --led-pwm-lsb-nanoseconds
        0,      // N/A --led-pwm-dither-bits

        50,     // --led-brightness

        0,      // N/A --led-scan-mode

        3,      // --led-row-addr-type

        0,      // N/A multiplexing
        0,      // N/A --led-hardware-pulse
        1,      // N/A --led-show-refresh
        0,      // N/A --led-inverse
        "BGR",  // --led-rgb-sequence
        NULL,   // N/A --led-pixel-mapper
        NULL,   // N/A --led-panel-type
        0       // N/A --led-limit-refresh
    };

    struct RGBLedRuntimeOptions matrix_runtime_opts = {
        4,      // --led-slowdown-gpio (used to be 4)
        0,      // N/A --led-daemon
        (bool)0,      // N/A do_gpio_init
        NULL,
        NULL
    };

    matrix = led_matrix_create_from_options_and_rt_options(&matrix_opts, &matrix_runtime_opts);
    frontCanvas = led_matrix_get_canvas(matrix);
    backCanvas = led_matrix_create_offscreen_canvas(matrix);
    led_canvas_fill(backCanvas, 0, 0, 0);
    led_canvas_fill(frontCanvas, 0, 0, 0);
}

/* Draw a full WIDTHxHEIGHT image provided here */
void MBV_write(uint8_t *image) {
    for (int i = 0; i < MBV_WIDTH * MBV_HEIGHT; ++i) {
        int x = i % MBV_WIDTH;
        int y = i / MBV_WIDTH + (x >= MBV_WIDTH / 2 ? MBV_HEIGHT : 0);

        led_canvas_set_pixel(backCanvas, x % (MBV_WIDTH / 2), y, *(image++), *(image++), *(image++));
    }
    backCanvas = led_matrix_swap_on_vsync(matrix, backCanvas);
}

void MBV_fill(uint8_t r, uint8_t g, uint8_t b) {
    led_canvas_fill(backCanvas, r, g, b);
    backCanvas = led_matrix_swap_on_vsync(matrix, backCanvas);
}

void MBV_close() {
    led_matrix_delete(matrix);
}
