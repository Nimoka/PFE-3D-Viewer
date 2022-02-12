#!/bin/bash

[ -d build ] || mkdir build
cd build

cmake .. -DBUILD_TESTING=ON
make
make tests

./tests/viewer/plyreader
