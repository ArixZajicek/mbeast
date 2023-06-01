#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>

class Config {
public:
  Config(int argc, char **argv);
  ~Config();

  bool isDebug() const;
  bool isSerialEnabled() const;

  int getInitSuccess() const;
private:
  bool debug;
  bool serialEnabled;
  
  int retVal;
};

#endif