#include <iostream>
#include <memory.h>
#include "states.hpp"

namespace State {
  Initial::Initial(const Context &ctx) : IState(ctx) {
    // Process initial data
  }

  void Initial::enter() {

  }

  void Initial::tick(const InputState &input, double d, Action &next) {
    //std::cout << "Ticking initial with time delta " << d << std::endl;
    if (input.keysDown[InputKey::BACK]) next.type = EXIT;
  }

  void Initial::draw(OutputState &out) {

  }

  void Initial::exit() {
    
  }
}