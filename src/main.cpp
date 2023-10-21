#include <iostream>
#include <chrono>
#include <cstring>
#include <vector>
#include <thread>
#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkStream.h"
#include "include/core/SkSurface.h"
#include "main.hpp"
#include "peripherals.hpp"
#include "states.hpp"

int run(const Config &cfg) {

  Serial *serial = nullptr;
  Visor *visor = nullptr;
  Input *input = nullptr;

  State::StateContext *ctxStack = nullptr;
  sk_sp<SkSurface> rasterSurface = nullptr;
  OutputState *outState = nullptr;

  LOG("Initializing peripherals");
  visor = new Visor();
  if (cfg.isSerialEnabled()) serial = new Serial(cfg.getSerialDevice());
  input = new Input(serial);

  LOG("Preparing output state");
  outState = new OutputState();

  LOG("Initializing context stack");
  ctxStack = new State::StateContext();
  ctxStack->state = new State::Initial(*ctxStack);

  LOG("Initializing Skia Canvas");
  outState->rawPix = (SkColor *)malloc(Visor::WIDTH * Visor::HEIGHT * sizeof(SkColor));
  rasterSurface = SkSurfaces::WrapPixels(
    SkImageInfo::MakeN32Premul(Visor::WIDTH, Visor::HEIGHT),
    outState->rawPix,
    Visor::WIDTH * sizeof(SkColor)
  );
  outState->cvs = rasterSurface->getCanvas();

  LOG("Preparing frame counter");
  auto frameDuration = std::chrono::nanoseconds(
    cfg.getFPS() > 0 ?
    static_cast<int>(round(1.0 / std::max(0, cfg.getFPS()) * 1000000000)) :
    1
  );

  auto frameEnd = std::chrono::steady_clock::now();
  auto frameStart = frameEnd;

  LOG("Beginning main loop");

  while (ctxStack != nullptr) {
    double lastFrameDelta = (frameEnd - frameStart).count() / 1000000000.0;
    frameStart = std::chrono::steady_clock::now();

    State::StateContext *nextCtx = ctxStack;
    input->tick();
    if (serial != nullptr) serial->tick(lastFrameDelta);
    ctxStack->state->tick(input->getResult(), lastFrameDelta, nextCtx);
    ctxStack->state->draw(*outState);

    visor->draw(outState->rawPix);
    visor->flip();

    State::StateContext *temp = nullptr;
    if (nextCtx != ctxStack) {
      if (ctxStack->parent == nextCtx) {
        // EXIT CURRENT CONTEXT
        LOG("Exiting current context");
        ctxStack->state->exit();
        delete ctxStack->state;
        delete ctxStack;
        ctxStack = nextCtx;
        if (ctxStack != nullptr) ctxStack->state->enter(true);
      } else if (nextCtx->parent == ctxStack) {
        // ENTER NEW CONTEXT
        LOG("Entering new context");
        ctxStack = nextCtx;
        ctxStack->state->enter(false);
      } else {
        ABORT("Invalid next context returned. Unable to continue.");
      }
    }

    auto actualFrameTime = std::chrono::steady_clock::now() - frameStart;
    if (actualFrameTime < frameDuration) {
      std::this_thread::sleep_for(frameDuration - (std::chrono::steady_clock::now() - frameStart));
    }
    frameEnd = std::chrono::steady_clock::now();
  }

  LOG("Deleting canvas");
  delete outState->cvs; // Also deletes the raster surface

  LOG("Deleting pixel buffer");
  delete outState->rawPix;

  LOG("Deleting output state");
  delete outState;

  LOG("Deleting context stack");
  delete ctxStack;

  LOG("Deleting input");
  delete input;

  LOG("Deleting Visor");
  delete visor;

  LOG("Deleting serial object");
  delete serial;

  LOG("Exiting run()");
  return 0;
}

int main(int argc, char **argv) {
  LOG("Starting mbeast");

  Config cfg(argc, argv);

  int argInit = cfg.getInitSuccess();
  if (argInit == 1) {
    return 0;
  } else if (argInit != 0) {
    return argInit;
  }

  LOG("Args parsed");

  run(cfg);

  LOG("Safely quitting application");
  return 0;
}