#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "matrix.h"

// Flags
unsigned int debug = 0;
unsigned int init_success = 0;
unsigned int flip_buffers = 0;

// SDL Relevant stuff
SDL_Window *sdl_win = NULL;
SDL_Surface *sdl_win_surface = NULL;
SDL_Surface *d_temp = NULL;

// Buffers
pixel *backbuffer = NULL;
pixel *frontbuffer = NULL;

// Matrix info
unsigned int width = 0;
unsigned int height = 0;

_DLL int matrix_init(uint matrix_width, uint matrix_height, byte debug_mode) {
    // Set globals
    debug = debug_mode;
    width = matrix_width;
    height = matrix_height;

    LOG("debug mode: %d", debug_mode);
    LOG("starting with dimensions %dx%d", width, height);

    // Initialize double buffers
    backbuffer = malloc(matrix_width * matrix_height * sizeof(pixel));
    frontbuffer = malloc(matrix_width * matrix_height * sizeof(pixel));
    LOG("finished allocating buffers");

    if (debug) {
        LOG("starting SDL window with resolution %dx%d", width, height);

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            LOG("SDL could not initialize! SDL_Error: %s", SDL_GetError());
            return -1;
        }

        sdl_win = SDL_CreateWindow(
            "HUB75 Matrix Debug Window",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height,
            SDL_WINDOW_SHOWN);

        if (sdl_win == NULL) {
            LOG("Window could not be created! SDL_Error: %s", SDL_GetError());
            return -1;
        }

        sdl_win_surface = SDL_GetWindowSurface(sdl_win);

        SDL_FillRect(sdl_win_surface, NULL, SDL_MapRGB(sdl_win_surface->format, 0xFF, 0xFF, 0xFF));
        SDL_UpdateWindowSurface(sdl_win);

        init_success = 1;
    } else {
        // TODO: Initialize HUB75 library
    }

    return 0;
}

_DLL void matrix_tick() {
    if (debug) {
        SDL_Event e;
        if (flip_buffers != 0) {
            LOG("beginning buffer flip");
            // Swap buffer pointers first
            pixel* temp_buffer = frontbuffer;
            frontbuffer = backbuffer;
            backbuffer = temp_buffer;

            d_temp = SDL_CreateRGBSurfaceFrom(
                frontbuffer,
                width, height,
                32,
                4 * width,
                0xFF, 0xFF00, 0xFF0000, 0x00);

            SDL_BlitSurface(d_temp, NULL, sdl_win_surface, NULL);
            SDL_FreeSurface(d_temp);

            flip_buffers = 0;
            LOG("buffer flip complete");
        }

        while (SDL_PollEvent(&e) != 0) {}

        SDL_UpdateWindowSurface(sdl_win);
    } else {
        // TODO: rpi_tick
    }
}

_DLL void matrix_put(pixel *image) {
    if (init_success == 0) {
        LOG("attempted to put pixels when not initialized, aborting");
        return;
    }

    memcpy(backbuffer, image, width * height * sizeof(pixel));
    LOG("copied new data into backbuffer");
}

_DLL void matrix_flip() {
    if (init_success == 0) {
        LOG("attempted to flip buffers when not initialized, aborting");
        return;
    }

    flip_buffers = 1;
}

_DLL void matrix_release() {
    if (init_success != 0) {
        init_success = 0;

        if (debug) {
            SDL_FreeSurface(sdl_win_surface);
            sdl_win_surface = NULL;
            SDL_DestroyWindow(sdl_win);
            sdl_win = NULL;
            SDL_Quit();
            SDL_DestroyWindow(sdl_win);
            SDL_VideoQuit();
        }

        if (backbuffer != NULL || frontbuffer != NULL) {
            free(backbuffer);
            free(frontbuffer);
            backbuffer = NULL;
            frontbuffer = NULL;
        }
    }
    flip_buffers = 0;
}
