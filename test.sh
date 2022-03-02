#!/bin/bash

[ -d build ] || mkdir build
cd build

cmake .. -DBUILD_LIBS=OFF -DBUILD_TESTING=ON -DDEBUG_ALL=ON
make
make tests

./tests/viewer/plyreader
