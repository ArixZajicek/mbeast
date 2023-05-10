#ifndef LIBMATRIX_H
#define LIBMATRIX_H

#include "common.h"

/// <summary>
/// Initializes the RGB matrix, buffers, and subsystem. Or, if debug is set,
/// will initialize SDL resources for debug mode.
/// </summary>
/// <param name="matrix_width">Width of matrix</param>
/// <param name="matrix_height">Height of matrix</param>
/// <param name="debug_mode">1 if SDL window should be used, 0 if not</param>
/// <returns>0 if successful, nonzero if failed</returns>
_DLL int matrix_init(uint matrix_width, uint matrix_height, byte debug_mode);

/// <summary>
/// Check if the matrix is running
/// </summary>
_DLL void matrix_tick();

/// <summary>
/// Put pixels into the backbuffer
/// </summary>
/// <param name="image">Pointer to the image data</param>
_DLL void matrix_put(pixel *image);

/// <summary>
/// Flip buffers and begin copying new data to matrix
/// </summary>
_DLL void matrix_flip();

/// <summary>
/// Release all resources
/// </summary>
_DLL void matrix_release();

#endif