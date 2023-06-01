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
    const Context &ctx;

  public:
    IState(const Context &context) : ctx(context) {}

    virtual void tick(const InputState &input, double delta, State::Action &next) = 0;

    virtual void enter() {};
    virtual void draw(OutputState &output) {};
    virtual void exit() {};
  };

  /******************************************************
   * Declarations for each state implementation
   ******************************************************/

  class Initial : public IState {
  public:
    Initial(const Context &ctx);
    void enter();
    void tick(const InputState &, double, State::Action &);
    void draw(OutputState &);
    void exit();
  };
}

#endif