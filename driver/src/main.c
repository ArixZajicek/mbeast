#include "mb.h"
#include <pthread.h>
#include <unistd.h>

void test_visor();

int main(int argc, char **argv) {

  // Create matrix
  MBV_create();
  test_visor();

  // Check for other peripherals

  // Create socket file if not exists

  // Begin listening for a client on socket
  while (1) {
    // Once client found, begin listening for matrix data


  }

  MBV_close();
  return 0;
}


void test_visor() {
  const int SIZE = MBV_WIDTH * MBV_HEIGHT * 3;
  char color[SIZE];

  // Black
  for (int i = 0; i < SIZE; i++) color[i] = 0;
  MBV_write(color);
  MBV_flip();
  sleep(0.25f);

  // R
  for (int i = 0; i < SIZE; i++) color[i] = i % 3 == 0 ? 0xFF : 0;
  MBV_write(color);
  MBV_flip();
  sleep(0.25f);

  // G
  for (int i = 0; i < SIZE; i++) color[i] = i % 3 == 1 ? 0xFF : 0;
  MBV_write(color);
  MBV_flip();
  sleep(0.25f);

  // B
  for (int i = 0; i < SIZE; i++) color[i] = i % 3 == 2 ? 0xFF : 0;
  MBV_write(color);
  MBV_flip();
  sleep(0.25f);

  // C
  for (int i = 0; i < SIZE; i++) color[i] = i % 3 != 0 ? 0xFF : 0;
  MBV_write(color);
  MBV_flip();
  sleep(0.25f);

  // M
  for (int i = 0; i < SIZE; i++) color[i] = i % 3 != 1 ? 0xFF : 0;
  MBV_write(color);
  MBV_flip();
  sleep(0.25f);

  // Y
  for (int i = 0; i < SIZE; i++) color[i] = i % 3 != 2 ? 0xFF : 0;
  MBV_write(color);
  MBV_flip();
  sleep(0.25f);

  // White
  for (int i = 0; i < SIZE; i++) color[i] = 0xFF;
  MBV_write(color);
  MBV_flip();
  sleep(0.25f);
}