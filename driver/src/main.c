#include <pthread.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>

#include "mb.h"

void test_visor();

int main(int argc, char **argv) {
  const char *SOCKET_PATH = "mbeast_driver";
  const int BUFFER_SIZE = 256;

  int srv, client;
  struct sockaddr_un srv_addr, client_addr;
  char buffer[BUFFER_SIZE];
  int result;



  // Create matrix
  MBV_create();
  test_visor();

  // Check for other peripherals (Pico/Arduino Serial)


  // Create server-side socket and listen for one client at a time:
  // https://www.educative.io/answers/unix-socket-programming-in-c
  srv = socket(AF_UNIX, SOCK_STREAM, 0);
  if (srv == -1) {
    ABORT("Could not create socket.");
  }

  srv_addr.sun_family = AF_UNIX;
  strcpy(srv_addr.sun_path, SOCKET_PATH);
  int slen = sizeof(srv_addr);
  bind(srv, (struct sockaddr *) &srv_addr, slen);
  listen(srv, 1);

  // Begin listening for a client on socket
  while (1) {
    // Once client found, begin listening for matrix data
        int clen = sizeof(client_addr);
        
        client = accept(srv, (struct sockaddr *) &client_addr, &clen);
        result = read(client, buffer, BUFFER_SIZE);
        uint8_t cmd = buffer[0];

        if (result == -1) {
          LOG("Failed to read command byte from client.");
        } else if (result == 0) {
          LOG("Empty read from client.");
        } else if (cmd == 0x00) {
          LOG("NOP received.");
        } else if (cmd == 0x10) {
          LOG("Clear displays with black.");
            MBV_fill(0, 0, 0);
        } else if (cmd == 0x11) {
          if (result < 4) {
            LOG("Not enough bytes received for single color.");
          } else {
            LOG("Fill displays with single color.");
            MBV_fill(buffer[1], buffer[2], buffer[3]);
          }
        } else {
          LOG("Unknown command %d", cmd);
        }

        write(client, "OK", 3);
        close(client);
  }

  sleep(5);

  MBV_close();
  return 0;
}


void test_visor() {
  const int SIZE = MBV_WIDTH * MBV_HEIGHT * 3;
  char color[SIZE];

  // Black
  MBV_fill(0, 0, 0);
  sleep(1);
  
  // R
  MBV_fill(0xFF, 0, 0);
  sleep(1);

  // G
  MBV_fill(0, 0xFF, 0);
  sleep(1);

  // B
  MBV_fill(0, 0, 0xFF);
  sleep(1);

  // C
  MBV_fill(0, 0xFF, 0xFF);
  sleep(1);

  // M
  MBV_fill(0xFF, 0, 0xFF);
  sleep(1);

  // Y
  MBV_fill(0, 0xFF, 0xFF);
  sleep(1);

  // White
  MBV_fill(0xFF, 0xFF, 0xFF);
  sleep(1);
}