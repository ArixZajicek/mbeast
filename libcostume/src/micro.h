#ifndef LIBMICRO_H
#define LIBMICRO_H

typedef unsigned char byte;

typedef struct micro_input_state {
    byte acc_x, acc_y, acc_z;
    byte gyro_x, gyro_y, gyro_z;
    byte boop;
    byte *digit_state;
    byte digit_count;
} micro_input_state;

typedef struct micro_output_state {
    byte ear_l, ear_r;
} micro_output_state;

// Initialize microcontroller. Returns nonzero if device failed to init.
int costume_micro_init(const char* device);

// Check if microcontroller is still working on the last operation.
// No new operations should be performed while this returns as nonzero.
int micro_busy();

// Triggers a new capture of input states (can't be busy)
// Returns nonzero if new capture failed, typically due to micro being busy
int micro_start_capture();

// Gets the last retrieved state. Safe to call while busy, will just return the last known state, null if error
micro_input_state *micro_get_state();

// Sends the microcontroller an output state (okay to be busy, nonzero if error)
int micro_put(micro_output_state *state);

// Free all resources used by the microcontroller
void micro_release();

#endif