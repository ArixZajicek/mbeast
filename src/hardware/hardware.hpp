#ifndef HARDWARE_HPP
#define HARDWARE_HPP

#include <inttypes.h>
#include <SDL2/SDL.h>

/**** UNIVERSAL STRUCTS ****/

struct Color {
  uint8_t r, g, b;
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

///////////////////
/**** CLASSES ****/
///////////////////

class Serial {
public:
  const double TIMEOUT_DURATION = 0.5;

  enum Status {
    QUEUED = 99,
    AWAITING_OK = 100,
    SENDING = 110,
    AWAITING_RESPONSE = 198,
    RECEIVING_RESPONSE = 199,
    RESPONSE_RECEIVED = 200,
    ERROR = 400,
    NOT_FOUND = 404,
  };

  struct Payload {
    uint16_t length;
    void *body;
  };

  Serial(const char *dev);
  void tick(double delta);
  uint32_t sendMessage(void *data, uint16_t len);
  Status getStatus(uint32_t id);
  Payload getResponse(uint32_t id);
private:
  struct Message {
    uint32_t id;
    Status status;
    Payload request, response;
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
  const int WIDTH = 256;
  const int HEIGHT = 64;

  Visor();
  void draw(Color *pixel);
  void flip();
  ~Visor();

private:
  // Flags
  bool initSuccess = 0;
  bool flipBuffers = 0;

  // SDL Relevant stuff
  SDL_Window *sdlWindow = NULL;
  SDL_Surface *sdlWindowSurface = NULL;
  SDL_Surface *drawSurface = NULL;

  // RGB Matrix Stuff
  struct RGBLedMatrix *matrix = NULL;
  struct LedCanvas *backCanvas, *frontCanvas;

  // Buffers
  Color *backBuffer = NULL;
  Color *frontBuffer = NULL;
};

class Output {
public:
  struct Ears {
    double leftRoll;
    double leftPitch;
    double rightRoll;
    double rightPitch;
  };

  struct State {
    Color *visor;
    Color *strips;
    Ears *ears;
  };

  Output(Serial *serial);
  void send(State &state);
};

class Input {
public:
  enum Key {
    UP, DOWN, LEFT, RIGHT, ACTION, BACK, BOOP, __COUNT
  };

  typedef bool Keys[Key::__COUNT];

  struct State {
    Keys keysDown;
    Keys keysTyped;

    Vector2 analog;
    Vector3 motion;
  };

  Input(Serial *serial);

  // We can't assume reading input will be faster than a frame. We just let this
  // abstract away any logic for that under the hood, call tick() once per frame,
  // and call getResult() each time. getResult() may still have old data, but we
  // can at least be sure it's valid (note that keysTyped still gets updated
  // regardless of if the data is new or not)
  void tick();
  State getResult();

private:
  uint32_t msg;
  Vector3 motion;
  Vector2 analog;
  Keys keysDown = {}, keysTyped = {};

  Serial *serial;

  const char msgReq = 'i';
  struct RawResponse {
    uint8_t acc_x, acc_y, acc_z, stick_x, stick_y, buttons;
  };
};

#endif