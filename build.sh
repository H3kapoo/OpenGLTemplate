#!/bin/sh
cmake -B simpleUI -GNinja .
cmake --build simpleUI
./debug/simpleUI