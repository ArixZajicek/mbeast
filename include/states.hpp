#ifndef STATES_HPP
#define STATES_HPP

#include <inttypes.h>
#include "hardware.hpp"

namespace State {
  // Forward declarations:
  class IState;

  struct StateContext {
    StateContext *parent;
    IState *state;
  };

  struct RenderContext {
    uint8_t pixels;
  };

  class IState {
  protected:
    StateContext &ctx;

  public:
    IState(StateContext &context) : ctx(context) {}

    virtual void tick(const InputState &input, double delta, StateContext *&nextCtx) = 0;

    virtual void enter(bool resume) {};
    virtual void draw(OutputState &output) {};
    virtual void exit() {};
  };

  /******************************************************
   * Declarations for each state implementation
   ******************************************************/

  class Initial : public IState {
  public:
    Initial(StateContext &ctx);
    void enter(bool);
    void tick(const InputState &, double, StateContext *&);
    void draw(OutputState &);
    void exit();
  };

  class Neutral : public IState {
  public:
    Neutral(StateContext &ctx);
    void enter(bool);
    void tick(const InputState &, double, StateContext *&);
    void draw(OutputState &);
    void exit();
  };
}

#endif