#!/bin/bash

[ -d build ] || mkdir build
cd build

cmake .. -Wno-dev -DBUILD_LIBS=OFF -DBUILD_TESTING=OFF
make
