#ifndef HARDWARE_HPP
#define HARDWARE_HPP

#include <inttypes.h>
#include "led-matrix.h"
#include "main.hpp"

class Serial {
public:
  const double TIMEOUT_DURATION = 0.5;

  Serial(const char *dev);
  ~Serial();

  void tick(double delta);

  uint32_t sendMessage(void *data, uint16_t len);
  SerialStatus getStatus(uint32_t id) const;
  SerialPayload getResponse(uint32_t id);

private:
  struct Message {
    uint32_t id;
    SerialStatus status;
    SerialPayload request, response;
    Message *next;
    uint16_t bytesRead;
    double emptyDuration;
  };

  uint32_t lastId = 1000;

  Message *list;
  Message *active;

  int port;
};

class Visor {
public:
  static const int WIDTH = 256;
  static const int HEIGHT = 64;

  Visor();
  ~Visor();

  void draw(SkColor *pixel);
  void flip();
};

class Input {
public:
  Input(Serial *serial = nullptr);

  // We can't assume reading input will be faster than a frame. We just let this
  // abstract away any logic for that under the hood, call tick() once per frame,
  // and call getResult() each time. getResult() may still have old data, but we
  // can at least be sure it's valid (note that keysTyped is still guaranteed to
  // only include new keys once per frame regardless of if the data is new or not)
  void tick();
  InputState getResult();

private:
  uint32_t msg;
  InputState state;
  InputKeys typed;

  Serial *serial;

  const char msgReq = 'i';
  struct RawResponse {
    uint8_t acc_x, acc_y, acc_z, stick_x, stick_y, buttons;
  };
};

#endif