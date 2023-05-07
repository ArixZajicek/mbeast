#ifndef LIBMATRIX_H
#define LIBMATRIX_H

#include <inttypes.h>
typedef uint8_t byte;
typedef unsigned int uint;
typedef struct pixel {
    byte r;
    byte g;
    byte b;
} pixel;


// Matrix initialization, width and height. Nonzero if failed.
__declspec(dllexport)
int matrix_init(uint matrix_width, uint matrix_height, byte debug_mode);

// Put pixels into the backbuffer
__declspec(dllexport)
int matrix_put(pixel *image);

// Flip buffers and begin copying new data to matrix
__declspec(dllexport)
int matrix_flip();

// Release all resources
__declspec(dllexport)
void matrix_release();




#endif