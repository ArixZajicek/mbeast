#ifndef MODELS_HPP
#define MODELS_HPP

#include <inttypes.h>
#include "led-matrix-c.h"

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

/* CUSTOM TYPES */

typedef bool InputKeys[InputKey::__COUNT];

/* STRUCTS */

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
  Color *visor;
  Color *strips;
  Ears ears;
};

#endif