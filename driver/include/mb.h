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

#define MBI_BTN_ACCEPT 0x01
#define MBI_BTN_CANCEL 0x02

typedef struct {
    uint8_t boop;
    uint8_t btn;
    int8_t acc_x, acc_y, acc_z;
} MBI_packet;

void MBI_init();
MBI_packet MBI_get();
void MBI_close();

#endif