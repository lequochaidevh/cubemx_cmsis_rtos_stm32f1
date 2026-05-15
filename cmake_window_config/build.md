```sh
cp -rf * stm32f103FreeRTOS_window/
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE="../cmake/gcc-arm-none-eabi.cmake" ..
cmake --build .
```