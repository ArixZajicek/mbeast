#ifndef MB_H
#define MB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define LOG(...) printf("[%s()]: ", __FUNCTION__); printf(__VA_ARGS__); printf("\n")
#define ABORT(...) printf("ERROR IN %s(): ", __FUNCTION__); printf(__VA_ARGS__); printf("\n"); exit(-1)

extern const int MBV_WIDTH;
extern const int MBV_HEIGHT;

void MBV_create();
void MBV_write(uint8_t *image);
void MBV_fill(uint8_t r, uint8_t g, uint8_t b);
void MBV_close();

#endif