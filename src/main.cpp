#include <iostream>
#include <chrono>
#include <cstring>
#include "config.hpp"
#include "debug.hpp"
#include "states.hpp"
#include "hardware.hpp"

#include "include/core/SkData.h"
#include "include/core/SkImage.h"
#include "include/core/SkStream.h"
#include "include/core/SkSurface.h"

#include <vector>

#ifdef HEADLESS
Visor *visor;
void initDisplay() {
  visor = new Visor();
}

void tickDisplay(double delta) {
  // Input stuff?
}

InputState getInputState() {
  return {};
}

void drawDisplay(OutputState &outState) {
  visor->draw(outState.rawPix);
}


void freeDisplay() {
  delete visor;
}

#else
Window *window;
void initDisplay() {
  window = new Window(Visor::WIDTH * 6, Visor::HEIGHT * 6);
}

void tickDisplay(double delta) {
  window->tick(delta);
}

InputState getInputState() {
  return window->getInputState();
}

void drawDisplay(OutputState &outState) {
  SDL_Surface *tempSurface = SDL_CreateRGBSurfaceFrom(
    outState.rawPix,
    Visor::WIDTH, Visor::HEIGHT,
    32, //24,
    Visor::WIDTH * sizeof(SkColor), //3 * Visor::WIDTH,
    0xFF0000, 0xFF00, 0xFF, 0xFF000000
  );

  SDL_Rect targetRect = { 0, 0, Visor::WIDTH * 6, Visor::HEIGHT * 6 };

  SDL_BlitScaled(tempSurface, nullptr, window->getBackBuffer(), &targetRect);

  SDL_FreeSurface(tempSurface);
  window->flip();
}

void freeDisplay() {
  delete window;
}
#endif

int run(const Config &cfg) {
  LOG("Initializing objects");

  // Hardware devices
  Serial *serial = nullptr;

  initDisplay();

  if (cfg.isSerialEnabled()) {
    serial = new Serial(cfg.getSerialDevice());
  }

  double targetFrametime = 0;
  if (cfg.getFPS() > 0) {
    targetFrametime = 1.0 / cfg.getFPS();
  }

  LOG("Window created");

  Input input(*serial);

  // Context stack
  State::StateContext *ctxStack = new State::StateContext();
  ctxStack->state = new State::Initial(*ctxStack);

  // We'll only use a single output state here
  SkImageInfo info = SkImageInfo::MakeN32Premul(Visor::WIDTH, Visor::HEIGHT);
  void *rawData = malloc(Visor::WIDTH * Visor::HEIGHT * sizeof(SkColor));
  sk_sp<SkSurface> rasterSurface = SkSurfaces::WrapPixels(info, rawData, Visor::WIDTH * sizeof(SkColor)); //SkSurfaces::Raster(info);
  SkCanvas *canvas = rasterSurface->getCanvas();

  OutputState outState = {
    cvs: canvas,
    rawPix : (SkColor *)rawData,
    ears : {0, 0, 0, 0},
  };

  LOG("Initialization complete");

  auto frameEnd = std::chrono::steady_clock::now();
  auto frameStart = frameEnd;

  LOG("Beginning main loop");

  while (ctxStack != nullptr) {
    double lastFrameDelta = (frameEnd - frameStart).count() / 1000000000.0;
    frameStart = std::chrono::steady_clock::now();

    // Tick main things
    tickDisplay(lastFrameDelta);

    State::StateContext *nextCtx = ctxStack;
    if (serial != nullptr) {
      input.tick();
      serial->tick(lastFrameDelta);
      ctxStack->state->tick(input.getResult(), lastFrameDelta, nextCtx);
    } else {
      ctxStack->state->tick(getInputState(), lastFrameDelta, nextCtx);
    }
    ctxStack->state->draw(outState);

    drawDisplay(outState);

    State::StateContext *temp = nullptr;
    if (nextCtx != ctxStack) {
      if (nextCtx->parent == ctxStack) {
        // ENTER NEW CONTEXT
        LOG("Entering new context");
        ctxStack = nextCtx;
        ctxStack->state->enter(false);
      } else if (ctxStack->parent == nextCtx) {
        // EXIT CURRENT CONTEXT
        LOG("Exiting current context");
        ctxStack->state->exit();
        delete ctxStack->state;
        delete ctxStack;
        ctxStack = nextCtx;
        ctxStack->state->enter(true);
      } else {
        ABORT("Invalid next context returned. Unable to continue.");
      }
    }

    double actualFrameTime = (std::chrono::steady_clock::now() - frameStart).count() / 1000000000.0;
    if (actualFrameTime < targetFrametime) {
      //SDL_Delay((targetFrametime - (std::chrono::steady_clock::now() - frameStart).count() / 1000000000.0) * 1000);
    }
    frameEnd = std::chrono::steady_clock::now();
  }

  LOG("Freeing hardware objects");

  delete serial;
  freeDisplay();
  delete outState.rawPix;
  delete outState.cvs;

  LOG("Exiting loop");
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