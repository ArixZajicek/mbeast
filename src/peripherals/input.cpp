#ifdef HEADLESS

#include <malloc.h>
#include <SDL2/SDL.h>
#include "main.hpp"
#include "peripherals.hpp"

SDL_GameController *gameController = nullptr;

Input::Input(Serial *s) {
  serial = s;
  msg = 0;
  state = {};
  for (int k = 0; k < InputKey::__COUNT; k++) {
    typed[k] = false;
  }

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
    LOG("Could not initialize SDL: %s", SDL_GetError());
  } else {
    LOG("Initialized SDL Controller input");
  }

  int numJoysticks = SDL_NumJoysticks();
  if (SDL_NumJoysticks() < 1) {
    LOG("No joysticks found (%d). Not initializing.", numJoysticks);
  } else {
    LOG("Initializing main controller.");
    gameController = SDL_GameControllerOpen(0);
    if (gameController == nullptr) {
      LOG("Could not initialize game controller 0: %s", SDL_GetError());
    }
  }
}

void Input::tick() {
  InputKeys newKeysDown;
  for (int k = 0; k < InputKey::__COUNT; k++) {
    newKeysDown[k] = state.keysDown[k];
  }

  //if (gameController != nullptr) {
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    switch (e.type) {
    case SDL_CONTROLLERBUTTONDOWN:
      LOG("Controller button %d pressed", e.cbutton.which);
      break;
    case SDL_JOYDEVICEADDED:
      LOG("Controller added! %d", e.cdevice.which);
      break;
    case SDL_JOYDEVICEREMOVED:
      LOG("Controller removed! %d", e.cdevice.which);
      break;
    }
  }
  //}

  if (serial == nullptr) return;

  // If we have an active message, see if it's ready
  if (msg != 0) {
    SerialStatus status = serial->getStatus(msg);
    if (status == SerialStatus::RESPONSE_RECEIVED) {
      SerialPayload resp = serial->getResponse(msg);
      if (resp.length == sizeof(RawResponse)) {
        RawResponse *raw = (RawResponse *)resp.body;
        newKeysDown[UP] = raw->buttons | (1 << UP);
        newKeysDown[DOWN] = raw->buttons | (1 << DOWN);
        newKeysDown[LEFT] = raw->buttons | (1 << LEFT);
        newKeysDown[RIGHT] = raw->buttons | (1 << RIGHT);
        newKeysDown[ACTION] = raw->buttons | (1 << ACTION);
        newKeysDown[BACK] = raw->buttons | (1 << BACK);
        newKeysDown[BOOP] = raw->buttons | (1 << BOOP);
        state.analog.x = (raw->stick_x / 127.5) - 127.5;
        state.analog.y = (raw->stick_y / 127.5) - 127.5;
        state.motion.x = (raw->acc_x / 127.5) - 127.5;
        state.motion.y = (raw->acc_y / 127.5) - 127.5;
        state.motion.z = (raw->acc_z / 127.5) - 127.5;
      } else {
        ABORT("Response format error: Expected length %lu but got length %d", sizeof(RawResponse), resp.length);
      }
      msg = 0;
    } else if (status == SerialStatus::ERROR || status == SerialStatus::NOT_FOUND) {
      msg = 0;
    }
  }

  if (msg == 0) {
    msg = serial->sendMessage((void *)&msgReq, 1);
  }

  // This will only set KeysTyped if there's something new, but never clear it
  for (int k = 0; k < InputKey::__COUNT; k++) {
    typed[k] |= !state.keysDown[k] && newKeysDown[k];
    state.keysDown[k] = newKeysDown[k];
  }
}

InputState Input::getResult() {
  // Update keys typed
  for (int k = 0; k < InputKey::__COUNT; k++) {
    state.keysTyped[k] = typed[k];
    typed[k] = false;
  }

  return state;
}

Input::~Input() {
  LOG("Exiting input object");
}

#endif