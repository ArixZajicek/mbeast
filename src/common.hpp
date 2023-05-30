
#ifndef COMMON_HPP
#define COMMON_HPP

#define DEBUG_LOG

// Easier consistent log formatting
#ifdef DEBUG_LOG
#define LOG(...) printf("[%s()] ", __FUNCTION__); printf(__VA_ARGS__); printf("\n")
#else
#define LOG(...) ;
#endif

static bool DEBUG = true;

#endif