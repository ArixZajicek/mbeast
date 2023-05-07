#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../libcostume/libcostume.h"


const uint WIDTH = 800, HEIGHT = 600;
const double MAX_DIST = 600;

double distance(double x1, double y1, double x2, double y2) {
    double dx = abs(x1 - x2);
    double dy = abs(y1 - y2);
    return sqrt(dx * dx + dy * dy);
}

double map_range(double x, double x_min, double x_max, double y_min, double y_max) {
    double t = (x - x_min) * (y_max - y_min) / (x_max - x_min) + y_min;
    if (t > y_max) return y_max;
    if (t < y_min) return y_min;
    return t;
}

uint8_t get_value(int x, int y, int tx, int ty) {
    return 255 - round(map_range(distance(x, y, tx, ty), 0, MAX_DIST, 0, 255));
}

void set_pixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t* a) {
    int i = (y * WIDTH + x) * 3;
    a[i] = r;
    a[i + 1] = g;
    a[i + 2] = b;
}

int main(int argc, char** argv) {
    printf("starting in main\n");

    matrix_init(WIDTH, HEIGHT, 1);

    uint8_t* pixels = malloc(WIDTH * HEIGHT * 3);

    // fill with random data
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            int i = (y * WIDTH + x) * 3;

            pixels[i + 2] = get_value(x, y, 0, 0);
            pixels[i + 1] = get_value(x, y, 800, 0);
            pixels[i] = get_value(x, y, 400, 600);
        }
    }

    matrix_put(pixels);

    matrix_flip();

    char* read_data = malloc(sizeof(char) * 80);

    scanf_s(read_data, 80);

    matrix_release();

    return 0;
}