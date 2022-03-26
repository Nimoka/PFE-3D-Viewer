#!/bin/bash

[ -d docs ] || mkdir docs

[ -d docs/viewer ] || mkdir docs/viewer
doxygen .doxygen-viewer
