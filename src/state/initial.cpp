#include <iostream>
#include <memory.h>
#include "states.hpp"

namespace State {
  Initial::Initial(Context *ctx) : IState(ctx) {
    // Process initial data
  }

  void Initial::enter() {

  }

  void Initial::tick(Input::State input, double d, Action next) {
    std::cout << "Ticking initial with time delta " << d << std::endl;
  }

  void Initial::draw(Output::State &out) {

  }

  void Initial::exit() {
    
  }
}