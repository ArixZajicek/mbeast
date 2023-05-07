#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "matrix.h"

// SDL2 only needed for debug mode, not needed for final build
#include <SDL2/SDL.h>

#define LOG(...) printf(__VA_ARGS__); printf("\n")

byte debug = 0;
SDL_Window *d_window = NULL;
SDL_Surface *d_surface = NULL;
SDL_Surface *d_temp = NULL;

// New thread info
byte running = 0;
byte flip_buffers = 0;
pthread_t *thread = NULL;

// Buffers
pixel *_buffer = NULL;
pixel *backbuffer = NULL;
pixel *frontbuffer = NULL;

// Matrix info
uint matrix_width = 0;
uint matrix_height = 0;

void *run(void *p) {
    printf("starting run\n");

    running = 1;
    while (running != 0) {
        if (flip_buffers) {
            pixel *old_frontbuffer = frontbuffer;
            frontbuffer = backbuffer;
            backbuffer = old_frontbuffer;
            
            if (debug) {
                printf("flip_buffers() debug\n");
                d_temp = SDL_CreateRGBSurfaceFrom(
                    frontbuffer, 
                    matrix_width, matrix_height, 
                    24, 
                    3 * matrix_width,
                    0xFF0000, 0xFF00, 0xFF, 0x00);
                
                SDL_BlitSurface(d_temp, NULL, d_surface, NULL);
                SDL_UpdateWindowSurface(d_window);
                SDL_FreeSurface(d_temp);
                printf("flip_buffers() finished\n");
            } else {
                // TODO: call display write on rpi library with new frontbuffer
            } 

            flip_buffers = 0;
        }

        if (debug) {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) running = 0;
            }
            SDL_Delay(16);
        } else {
            // Otherwise, update/refresh lib? gotta look into hub75 lib
        }
    }
    //Deallocate surface
    SDL_FreeSurface(d_surface);
    d_surface = NULL;

    //Destroy window
    SDL_DestroyWindow(d_window);
    d_window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

__declspec(dllexport)
int matrix_init(uint width, uint height, byte debug_mode) {
    // Prevent multiple instances
    if (thread != NULL) {
        return -1;
    }

    printf("matrix_init: starting with dimensions %dx%d\n", width, height);

    debug = debug_mode;

    // Set matrix details
    matrix_width = width;
    matrix_height = height;

    // Initialize double buffers
    _buffer = malloc(matrix_width * matrix_height * sizeof(pixel) * 2);
    backbuffer = _buffer;
    frontbuffer = _buffer + matrix_width * matrix_height * sizeof(pixel);

    printf("finished allocation buffer\n");

    if (debug) { // SDL init
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
            return -1;
        }
        
        printf("starting window with resolution %dx%d\n", matrix_width, matrix_height);
        d_window = SDL_CreateWindow(
            "HUB75 Matrix Debug Window",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            matrix_width, matrix_height,
            SDL_WINDOW_SHOWN);
        
        if (d_window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return -1;
        }

        d_surface = SDL_GetWindowSurface(d_window);

        printf("sdl_init_complete\n");

        //Fill the surface white
        SDL_FillRect(d_surface, NULL, SDL_MapRGB(d_surface->format, 0xFF, 0xFF, 0xFF));
        
        //Update the surface
        SDL_UpdateWindowSurface(d_window);
    } else {
        // TODO: Initialize HUB75 library
    }

    // Launch thread
    thread = malloc(sizeof(pthread_t));
    pthread_create(thread, NULL, run, NULL);

    printf("new thread started\n");

    return 0;
}

__declspec(dllexport)
int matrix_put(pixel *image) {
    // Don't allow new data while still flipping the buffers.
    if (flip_buffers != 0) {
        printf("WARNING: attempted to put pixel data while flipping buffers\n");
        return -1;
    }

    // Copy
    memcpy(backbuffer, image, matrix_width * matrix_height * sizeof(pixel));
    LOG("copied new %d data into backbuffer", matrix_width * matrix_height * sizeof(pixel));
    return 0;
}

__declspec(dllexport)
int matrix_flip() {
    flip_buffers = 1;
    return 0;
}

__declspec(dllexport)
void matrix_release() {
    running = 0;
    pthread_join(*thread, NULL);
    thread = NULL;

    if (debug) {
        SDL_DestroyWindow(d_window);
        SDL_Quit();
    }

    free(_buffer);
    _buffer = NULL;
    backbuffer = NULL;
    frontbuffer = NULL;
    flip_buffers = 0;
}
