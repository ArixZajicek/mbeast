#ifndef MB_H
#define MB_H

#define LOG(...) printf("[%s()]: ", __FUNCTION__); printf(__VA_ARGS__); printf("\n")
#define ABORT(...) printf("ERROR IN %s(): ", __FUNCTION__); printf(__VA_ARGS__); printf("\n"); exit(-1)

extern const int MBV_WIDTH;
extern const int MBV_HEIGHT;

void MBV_create();
void MBV_write(char *image);
void MBV_flip();
void MBV_close();

#endif