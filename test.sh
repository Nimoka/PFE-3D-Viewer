#!/bin/bash

[ -d build ] || mkdir build
cd build

cmake .. -DCMAKE_BUILD_TYPE=DEBUG \
		-DBUILD_LIBS=OFF -DBUILD_TESTING=ON -DDEBUG_ALL=ON
make
make tests

[ -f tests/viewer/plyreader ] && ./tests/viewer/plyreader
[ -f tests/viewer/cliloader ] && ./tests/viewer/cliloader
