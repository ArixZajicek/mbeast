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
  std::cout << "  -f 60, --fps=60                 Set the FPS limit. Default is 60. Values under 1" << std::endl;
  std::cout << "                               remove the limit entirely (not recommended)" << std::endl;
}

Config::Config(int argc, char **args) {
  LOG("Config object initializing");
  debug = false;
  serialEnabled = true;
  serialDevice = "/dev/ttyUSB0";
  fps = 60;

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
        } else if (strcmp(arg, "no-serial") == 0) {
          serialEnabled = false;
        } else if (strncmp(arg, "serial=", 7) == 0) {
          if (strlen(arg) > 7) {
            serialDevice = arg + 7;
          } else {
            printf("--serial= is missing serial device\n");
            retVal = -1;
            return;
          }
        } else if (strncmp(arg, "fps=", 4) == 0) {
          if (strlen(arg) > 4) {
            fps = atoi(arg + 4);
          } else {
            printf("--fps= is missing FPS\n");
            retVal = -1;
            return;
          }
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
          case 's':
            if (*(c + 1) == '\0' && i + 1 < argc && args[i + 1][0] != '-') {
              serialDevice = (args[++i]);
            } else {
              printf("-s is missing serial device\n");
              retVal = -1;
              return;
            }
            break;
          case 'f':
            if (*(c + 1) == '\0' && i + 1 < argc && args[i + 1][0] != '-') {
              fps = atoi(args[++i]);
            } else {
              printf("-f is missing FPS\n");
              retVal = -1;
              return;
            }
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
const char *Config::getSerialDevice() const { return serialDevice; }
int Config::getFPS() const { return fps; }

int Config::getInitSuccess() const { return retVal; }