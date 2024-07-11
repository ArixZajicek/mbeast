#include "mb.h"

void MBI_init() {
    // Init Serial
}

MBI_packet MBI_get() {
    // Get random data and insert it for now
    MBI_packet packet;
    packet.acc_x = 0;
    packet.acc_y = 0;
    packet.acc_z = 0;
    packet.boop = 50;
    packet.btn = 0;
    return packet;
}

void MBI_close() {
    // Close serial
}