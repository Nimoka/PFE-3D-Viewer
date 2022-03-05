#!/bin/bash

[ -d build ] || mkdir build
cd build

cmake .. -Wno-dev -DCMAKE_BUILD_TYPE=RELEASE \
		-DBUILD_LIBS=ON -DBUILD_TESTING=OFF -DDEBUG_ALL=OFF
make
