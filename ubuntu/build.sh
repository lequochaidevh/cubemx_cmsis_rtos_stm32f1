#!/bin/bash

echo "--- Ubuntu 20 build with CMake ---"

mkdir stm32f103c8t6_build_v0.0.1

cp -rf ../stm32f103FreeRTOS_window/* stm32f103c8t6_build_v0.0.1/

cp -rf cmake stm32f103c8t6_build_v0.0.1/

cd stm32f103c8t6_build_v0.0.1/

rm -rf build/

mkdir build/

cd build/

# Direct to host gcc-cross-9.2.1
# Todo: add config gcc
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/gcc-arm-none-eabi.cmake ..

make
