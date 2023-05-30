#include <iostream>

#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "hardware.hpp"

Serial::Serial(const char *dev) {
  port = open(dev, O_RDWR);

  if (port < 0) {
    throw std::runtime_error("Failed to open device (" + std::to_string(errno) + "): " + strerror(errno));
  }

  termios tty;

  if (tcgetattr(port, &tty) != 0) {
    throw std::runtime_error("Failure from tcgetattr (" + std::to_string(errno) + "): " + strerror(errno));
  }

  tty.c_cflag &= ~PARENB;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;
  tty.c_cflag &= ~CRTSCTS;
  tty.c_cflag |= CREAD;
  tty.c_cflag |= CLOCAL;

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO;
  tty.c_lflag &= ~ECHOE;
  tty.c_lflag &= ~ECHONL;
  tty.c_lflag &= ~ISIG;

  tty.c_iflag &= ~(IXON | IXOFF | IXANY);
  tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

  tty.c_oflag &= ~OPOST;
  tty.c_oflag &= ~ONLCR;

  // No blocking
  tty.c_cc[VTIME] = 0;
  tty.c_cc[VMIN] = 0;

  cfsetispeed(&tty, B115200);
  cfsetospeed(&tty, B115200);

  if (tcsetattr(port, TCSANOW, &tty) != 0) {
    throw std::runtime_error("Failure from tcsetattr (" + std::to_string(errno) + "): " + strerror(errno));
  }
}

void Serial::tick(double delta) {
  // Loop through messages 
  bool reloop = true;
  while (port >= 0 && active != nullptr && reloop) {
    reloop = false;
    // Should probably implement a READY ping instead of just sending right away.
    if (active->status == QUEUED) {
      active->status = SENDING;
    }

    if (active->status == SENDING) {
      write(port, &(active->request.length), sizeof(uint16_t));
      write(port, active->request.body, active->request.length);
      active->status = AWAITING_RESPONSE;
    }

    if (active->status == AWAITING_RESPONSE) {
      int available, bytesRead;
      ioctl(port, FIONREAD, &available);
      if (available >= 2 && (bytesRead = read(port, &(active->response.length), sizeof(uint16_t))) == 2) {
        active->response.body = malloc(active->response.length);
        active->emptyDuration = 0.0;
        active->status = RECEIVING_RESPONSE;
      } else if (available == 0 || bytesRead == 0) {
        active->emptyDuration += delta;
      } else if (available < 0) {
        active->status == ERROR;
        throw std::runtime_error("Failure retrieving response size (available=" + std::to_string(available) + ", read=" + std::to_string(bytesRead) + ")");
      }
    }

    if (active->status == RECEIVING_RESPONSE) {
      int bytesRead = read(port, active->response.body + active->bytesRead, active->response.length - active->bytesRead);
      if (bytesRead > 0) {
        active->emptyDuration = 0.0;
        active->bytesRead += bytesRead;
      } else if (bytesRead == 0) {
        active->emptyDuration += delta;
      } else if (bytesRead < 0) {
        active->status == ERROR;
        throw std::runtime_error("Failure retrieving response data (bytesRead=" + std::to_string(bytesRead) + ")");
      }

      if (active->bytesRead == active->response.length) {
        active->status = RESPONSE_RECEIVED;
      }
    }

    if (active->emptyDuration > TIMEOUT_DURATION) {
      active->status = ERROR;
      throw std::runtime_error("Message took too long to complete (" + std::to_string(active->emptyDuration) + " seconds)");
    }

    if (active->status == RESPONSE_RECEIVED || active->status == ERROR) {
      active = active->next;
      reloop = true;
    }
  }
}

uint32_t Serial::sendMessage(void *data, uint16_t len) {
  Message *msg = new Message{
    id: ++lastId,
    status : QUEUED,
    request : {length: len, body : data},
    response : {length: 0, body : nullptr},
    next : nullptr,
    bytesRead : 0,
    emptyDuration : 0.0,
  };

  if (list == nullptr) {
    list = msg;
  } else {
    Message *cur = list;
    while (cur->next != nullptr) cur = cur->next;
    cur->next = msg;
  }

  if (active == nullptr) {
    active = msg;
  }
}

Serial::Status Serial::getStatus(uint32_t id) {
  Message *cur = list;
  while (cur->id != id && cur->next != nullptr) cur = cur->next;

  if (cur == nullptr) {
    return NOT_FOUND;
  } else {
    return cur->status;
  }
}

Serial::Payload Serial::getResponse(uint32_t id) {
  Message *cur = list;
  while (cur->id != id && cur->next != nullptr) cur = cur->next;

  if (cur == nullptr || cur->status < 200) {
    return { 0, nullptr };
  } else {
    list = cur->next;
    Payload payload(cur->response);
    delete cur;
    return payload;
  }
}

Serial::~Serial() {
  close(port);
}
