#include <string.h>
#include "config.hpp"
#include "debug.hpp"

static void printHelp() {
  std::cout << "Using mbeast" << std::endl << std::endl;

  std::cout << "mbeast coordinates a variety of inputs and outputs to provide interactive and" << std::endl;
  std::cout << "fluid responses in realtime." << std::endl << std::endl;

  std::cout << "  -h, --help                      This help display" << std::endl;
  std::cout << "  -d, --debug                     Log debug output" << std::endl;
  std::cout << "  -s device, --serial=device      Specify device to use. Default is /dev/ttyUSB0" << std::endl;
  std::cout << "  -S, --no-serial                 Disable all serial communication" << std::endl;
}

Config::Config(int argc, char **args) {
  LOG("Config object initializing");
  debug = false;
  serialEnabled = true;

  for (int i = 1; i < argc; i++) {
    if (strlen(args[i]) < 2) {
      printf("Unknown argument '%s'\n", args[i]);
      retVal = -1;
      return;
    }

    if (args[i][0] == '-') {
      if (args[i][1] == '-') {
        // Arguments of --argument format (or just --, special case)
        char *arg = args[i] + 2;
        if (strcmp(arg, "help") == 0) {
          printHelp();
          retVal = 1;
          return;
        } else if (strcmp(arg, "debug") == 0) {
          debug = true;
        }if (strcmp(arg, "no-serial") == 0) {
          serialEnabled = false;
        } else {
          printf("Unknown argument '%s'\n", args[i]);
          retVal = -1;
          return;
        }

      } else {
        // Arguments of -a format
        for (char *c = args[i] + 1; *c != '\0'; c += 1) {
          switch (*c) {
          case 'h':
            printHelp();
            retVal = 1;
            return;
          case 'd':
            debug = true;
            break;
          case 'S':
            serialEnabled = false;
            break;
          default:
            printf("Unknown argument '-%c'\n", *c);
            retVal = -1;
            return;
          }
        }
      }
    } else {
      printf("Unknown argument '%s'\n", args[i]);
      retVal = -1;
      return;
    }
  }

  retVal = 0;
}

Config::~Config() {
  LOG("Config object exiting");
}

bool Config::isDebug() const { return debug; }
bool Config::isSerialEnabled() const { return serialEnabled; }

int Config::getInitSuccess() const { return retVal; }