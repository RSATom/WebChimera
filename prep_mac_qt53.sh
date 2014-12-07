#!/bin/bash
deps/firebreath/prepmac.sh src build_mac -D CMAKE_OSX_ARCHITECTURES="x86_64" -DCMAKE_PREFIX_PATH=~/Qt/5.3/clang_64 -Wno-dev
