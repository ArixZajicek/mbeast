#ifndef STATES_HPP
#define STATES_HPP

#include <inttypes.h>
#include "hardware.hpp"

namespace State {
  // Forward declarations:
  class IState;

  enum ActionType {
    NOP,
    ENTER,
    EXIT,
    SWAP,
  };

  struct Context {
    Context *parent;
    IState *state;
  };

  struct Action {
    ActionType type;
    Context *context;
  };

  struct RenderContext {
    uint8_t pixels;
  };

  class IState {
  protected:
    Context *context;

  public:
    IState(Context *context) {
      this->context = context;
      this->context->state = this;
    }

    virtual void tick(InputState input, double delta, State::Action &next) = 0;

    virtual void enter() {};
    virtual void draw(OutputState &output) {};
    virtual void exit() {};
  };

  /******************************************************
   * Declarations for each state implementation
   ******************************************************/

  class Initial : public IState {
  public:
    Initial(Context *ctx);
    void enter();
    void tick(InputState, double, State::Action &) override;
    void draw(OutputState &);
    void exit();
  };
}

#endif