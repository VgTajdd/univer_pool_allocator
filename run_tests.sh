#!/bin/bash

cmake --build build/tests --target test_template -j
cd build && ctest --output-on-failure && cd ..
