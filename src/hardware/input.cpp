#include "hardware.hpp"

#include <malloc.h>
#include <stdexcept>

Input::Input(Serial *serial) {
  this->serial = serial;
  this->msg = 0;
}

void Input::tick() {
  Keys newKeysDown;
  for (int k = 0; k < Key::__COUNT; k++) {
    newKeysDown[k] = keysDown[k];
  }

  // If we have an active message, see if it's ready
  if (msg != 0) {
    Serial::Status status = serial->getStatus(msg);
    if (status == Serial::Status::RESPONSE_RECEIVED) {
      Serial::Payload resp = serial->getResponse(msg);
      if (resp.length == sizeof(RawResponse)) {
        RawResponse *raw = (RawResponse *)resp.body;
        newKeysDown[UP] = raw->buttons | (1 << UP);
        newKeysDown[DOWN] = raw->buttons | (1 << DOWN);
        newKeysDown[LEFT] = raw->buttons | (1 << LEFT);
        newKeysDown[RIGHT] = raw->buttons | (1 << RIGHT);
        newKeysDown[ACTION] = raw->buttons | (1 << ACTION);
        newKeysDown[BACK] = raw->buttons | (1 << BACK);
        newKeysDown[BOOP] = raw->buttons | (1 << BOOP);
        analog.x = (raw->stick_x / 127.5) - 127.5;
        analog.y = (raw->stick_y / 127.5) - 127.5;
        motion.x = (raw->acc_x / 127.5) - 127.5;
        motion.y = (raw->acc_y / 127.5) - 127.5;
        motion.z = (raw->acc_z / 127.5) - 127.5;
      } else {
        throw std::runtime_error("Response format error: Expected length " + std::to_string(sizeof(RawResponse)) + " but got length " + std::to_string(resp.length));
      }
      msg = 0;
    } else if (status == Serial::Status::ERROR || status == Serial::Status::NOT_FOUND) {
      msg = 0;
    }
  }

  if (msg == 0) {
    msg = serial->sendMessage((void *)&msgReq, 1);
  }

  // This will only set KeysTyped if there's something new, but never clear it
  for (int k = 0; k < Key::__COUNT; k++) {
    keysTyped[k] |= !keysDown[k] && newKeysDown[k];
    keysDown[k] = newKeysDown[k];
  }
}

Input::State Input::getResult() {
  Keys keysTypedCopy;

  // Reset keys typed
  for (int k = 0; k < Key::__COUNT; k++) {
    keysTypedCopy[k] = keysTyped[k];
    keysTyped[k] = false;
  }

  Input::State returnState = {
    keysDown: keysDown,
    keysTyped: keysTypedCopy,
    analog: analog,
    motion: motion,
  };

  return returnState;
}