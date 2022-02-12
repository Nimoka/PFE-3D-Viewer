#!/bin/bash

[ -d build ] || mkdir build
cd build

cmake .. -DBUILD_TESTING=OFF
make
