#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>

#define LOG(...) printf("[%s()]: ", __FUNCTION__); printf(__VA_ARGS__); printf("\n")
#define ABORT(...) printf("ERROR IN %s(): ", __FUNCTION__); printf(__VA_ARGS__); printf("\n"); exit(-1)
/*
void LOG(const char *args, ...);
void ABORT(const char *args, ...);
*/

#endif