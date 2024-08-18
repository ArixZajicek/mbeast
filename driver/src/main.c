#include <pthread.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include "mb.h"

void test_visor();

int main(int argc, char **argv) {
    const char *SOCKET_PATH = "/tmp/mbeast_driver.socket";
    const int BUFFER_SIZE = 65536;

    int srv, client;
    struct sockaddr_un srv_addr, client_addr;
    char buffer[BUFFER_SIZE];
    int status;



    // Create matrix
    MBV_create();
    //test_visor();

    // Check for other peripherals (Pico/Arduino Serial)


    // Create server-side socket and listen for one client at a time:
    // https://www.educative.io/answers/unix-socket-programming-in-c
    if (access(SOCKET_PATH, F_OK) != -1) {
        if (unlink(SOCKET_PATH) == -1) {
            ABORT("Could not remove existing socket. Is the server already running?");
        }
    }

    srv = socket(AF_UNIX, SOCK_STREAM, 0);
    if (srv == -1) {
        ABORT("Could not create socket.");
    }

    srv_addr.sun_family = AF_UNIX;
    strcpy(srv_addr.sun_path, SOCKET_PATH);
    int slen = sizeof(srv_addr);

    status = bind(srv, (struct sockaddr *)&srv_addr, slen);
    if (status == -1) {
        ABORT("Could not bind to socket.");
    }

    // Accessible to non-sudo
    fchmod(srv, S_IROTH | S_IWOTH);

    status = listen(srv, 1);
    if (status == -1) {
        ABORT("Could not listen to bound socket.");
    }

    // Begin listening for a client on socket
    uint8_t wait_for_next_client = 1;
    while (wait_for_next_client == 1) {
        // Once client found, begin listening for matrix data
        int clen = sizeof(client_addr);

        client = accept(srv, (struct sockaddr *)&client_addr, &clen);
        if (client == -1) {
            ABORT("Failed to accept a client connection.");
        }

        // Client found. Read forever, until it disconnects.
        while (1) {
            status = read(client, buffer, BUFFER_SIZE);
            uint8_t cmd = buffer[0];

            if (status == -1) {
                LOG("Failed to read command byte from client.");
                break;
            
            } else if (status == 0) {
                LOG("Empty read from client. Disconnected.");
                break;

            } else if (cmd == 0x00) {
                LOG("NOP received.");
            
            } else if (cmd == 0x10) {
                LOG("Clear displays with black.");
                MBV_fill(0, 0, 0);

            } else if (cmd == 0x11) {
                if (status < 4) {
                    LOG("Not enough bytes received for single color.");
                } else {
                    LOG("Fill displays with single color.");
                    MBV_fill(buffer[1], buffer[2], buffer[3]);
                }
            
            } else if (cmd == 0x20) {
                int cnt = status;
                LOG("Received %d bytes on first read.", status);
                while (cnt < MBV_WIDTH * MBV_HEIGHT * 3 + 1 && status != -1) {
                    status = read(client, buffer + cnt, BUFFER_SIZE - cnt);
                    if (status > 0) {
                        cnt += status;
                        LOG("Received %d more bytes.", status);
                    } else {
                        LOG("Received status: %d", status);
                    }
                }

                if (cnt < MBV_WIDTH * MBV_HEIGHT * 3 + 1) {
                    LOG("Not enough bytes received for image.");
                } else {
                    MBV_write(buffer + 1);
                }

            } else if (cmd == 0x80) {
                MBI_packet p = MBI_get();
                write(client, &p, sizeof(p));

            } else if (cmd == 0xF0) {
                LOG("Quitting.");
                wait_for_next_client = 0;
                break;

            } else {
                LOG("Unknown command '%d'.", cmd);
            }
        }

        close(client);
    }

    close(srv);

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