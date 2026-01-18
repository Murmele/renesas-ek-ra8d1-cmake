#!/bin/bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=toolchain-arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo