#!/bin/bash
deps/firebreath/prepmac.sh src build_mac -D CMAKE_OSX_ARCHITECTURES="x86_64" -DCMAKE_PREFIX_PATH=~/Qt-5.4.0-static -DQT_STATIC=1 -Wno-dev
