#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>

class Config {
public:
  Config(int argc, char **argv);
  ~Config();

  bool isDebug() const;
  bool isSerialEnabled() const;
  const char* getSerialDevice() const;
  int getFPS() const;

  int getInitSuccess() const;
private:
  bool debug;
  bool serialEnabled;
  const char *serialDevice;
  int fps;
  
  int retVal;
};

#endif