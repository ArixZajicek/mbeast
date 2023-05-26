
#ifndef COMMON_H
#define COMMON_H

#include <inttypes.h>

// Should Debug logs be generated?
//#define DEBUG_LOG

// Required for MSVC DLL functions
#ifdef _MSC_VER
    #define _DLL __declspec(dllexport)
#else
    #define _DLL
#endif

// Easier consistent log formatting
#ifdef DEBUG_LOG
    #define LOG(...) printf("[%s()] ", __FUNCTION__); printf(__VA_ARGS__); printf("\n")
#else
    #define LOG(...) ;
#endif

typedef uint8_t byte;
typedef unsigned int uint;

// RGBA Pixel Format
typedef struct pixel {
    byte a;
    byte b;
    byte g;
    byte r;
} pixel;

#endif