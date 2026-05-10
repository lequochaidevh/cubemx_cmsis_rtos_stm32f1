#!/bin/bash

echo "--- Ubuntu 20 build with CMake ---"

cd ../stm32f103FreeRTOS_window/

rm -rf build/

mkdir build/

cd build/

# Direct to host gcc-cross-9.2.1
# Todo: add config gcc
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/gcc-arm-none-eabi.cmake ..

make
