#include <iostream>
#include <memory.h>
#include "states.hpp"

namespace State {
  class Initial : public IState {
    Initial(Context *ctx) : IState(ctx) {
      // Process initial data
    }

    void tick(Input::State input, double d, Action next) override {
      std::cout << "Ticking initial with time delta " << d << std::endl;

    }

    void draw(Output::State &out) {

    }
  };
}