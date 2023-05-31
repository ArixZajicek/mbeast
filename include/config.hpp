#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>

class ConfigObj {
public:
  int init(int argc, char **argv);

  bool isDebug();
  bool isSerialEnabled();

private:
  bool debug;
  bool serialEnabled;
};

static ConfigObj Cfg;

#endif