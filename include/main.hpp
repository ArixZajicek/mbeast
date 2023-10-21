#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <inttypes.h>
#include "include/core/SkCanvas.h"

/* CONFIG CLASS */

class Config {
public:
  Config(int argc, char **argv);
  ~Config();

  bool isDebug() const;
  bool isSerialEnabled() const;
  const char *getSerialDevice() const;
  int getFPS() const;

  int getInitSuccess() const;
private:
  bool debug;
  bool serialEnabled;
  const char *serialDevice;
  int fps;
  int retVal;
};

/* DEBUG HELPERS */

#define LOG(...) printf("[%s()]: ", __FUNCTION__); printf(__VA_ARGS__); printf("\n")
#define ABORT(...) printf("ERROR IN %s(): ", __FUNCTION__); printf(__VA_ARGS__); printf("\n"); exit(-1)

/* ENUMERATIONS */

enum InputKey {
  UP, DOWN, LEFT, RIGHT, ACTION, BACK, BOOP, __COUNT
};

enum SerialStatus {
  QUEUED = 99,
  AWAITING_OK = 100,
  SENDING = 110,
  AWAITING_RESPONSE = 198,
  RECEIVING_RESPONSE = 199,
  RESPONSE_RECEIVED = 200,
  ERROR = 400,
  NOT_FOUND = 404,
};

/* TYPES AND STRUCTS */

typedef bool InputKeys[InputKey::__COUNT];

struct SerialPayload {
  uint16_t length;
  void *body;
};

struct Vector3 {
  double x;
  double y;
  double z;
};

struct Vector2 {
  double x;
  double y;
};

struct InputState {
  InputKeys keysDown;
  InputKeys keysTyped;

  Vector2 analog;
  Vector3 motion;
};

struct Ears {
  double leftRoll;
  double leftPitch;
  double rightRoll;
  double rightPitch;
};

struct OutputState {
  SkCanvas *cvs;
  SkColor *rawPix;
  SkColor *strips;
  Ears ears;
};

#endif