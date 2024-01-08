#!/bin/sh
cmake -B build -GNinja . # generate ninja data
cmake --build build      # link and compile
mv debug/UnSnapshot_v1.0.0 .      # move out of debug folder
# ./simpleUI               # run at root level