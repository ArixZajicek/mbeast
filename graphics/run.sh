#!/bin/bash

# Start fake display so processing will run
Xvfb :1 -screen 0 800x600x24+32 &
export DISPLAY=:1
/home/arix/mbeast/graphics/build/src

