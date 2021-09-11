#!/bin/sh

cmake -DCMAKE_BUILD_TYPE=Release . -Bbuild-release
cmake --build build-release
