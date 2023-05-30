#include "hardware.hpp"

#include <malloc.h>
#include <iostream>

Input::Input(Serial *s) {
  serial = s;
  msg = 0;
  state = {};
  for (int k = 0; k < Key::__COUNT; k++) {
    typed[k] = false;
  }
}

void Input::tick() {
  Keys newKeysDown;
  for (int k = 0; k < Key::__COUNT; k++) {
    newKeysDown[k] = state.keysDown[k];
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
        state.analog.x = (raw->stick_x / 127.5) - 127.5;
        state.analog.y = (raw->stick_y / 127.5) - 127.5;
        state.motion.x = (raw->acc_x / 127.5) - 127.5;
        state.motion.y = (raw->acc_y / 127.5) - 127.5;
        state.motion.z = (raw->acc_z / 127.5) - 127.5;
      } else {
        std::cerr << "Response format error: Expected length " << std::to_string(sizeof(RawResponse)) << " but got length " << std::to_string(resp.length) << std::endl;
        exit(-1);
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
    typed[k] |= !state.keysDown[k] && newKeysDown[k];
    state.keysDown[k] = newKeysDown[k];
  }
}

Input::State Input::getResult() {
  // Update keys typed
  for (int k = 0; k < Key::__COUNT; k++) {
    state.keysTyped[k] = typed[k];
    typed[k] = false;
  }

  return state;
}