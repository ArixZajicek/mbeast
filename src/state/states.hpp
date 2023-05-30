

#ifndef STATES_HPP
#define STATES_HPP

#include <inttypes.h>

#include "hardware/hardware.hpp"

namespace State {

  enum ActionType {
    NOP,
    ENTER,
    EXIT,
    SWAP,
  };

  struct Action {
    ActionType type;
    Context *context;
  };

  struct Context {
    Context *parent;
    IState *state;
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

    virtual void tick(Input::State input, double delta, State::Action next) = 0;

    virtual void enter() {};
    virtual void draw(Output::State &output) {};
    virtual void exit() {};
  };


  /******************************************************
   * Declarations for each state implementation
   ******************************************************/

  class Initial : public IState {
    Initial(Context *ctx);
    void enter();
    void tick(Input::State, double, State::Action);
    void draw(Output::State &);
    void exit();
  };



}

#endif