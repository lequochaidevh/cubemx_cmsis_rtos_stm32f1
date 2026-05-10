# STM32F103C8T6 CMSIS v1 FreeRTOS

## Setup

### Start project in Window host.

Install:
```sh
#  Build and Flash code.
CUBEIDE
#  Generate code.
CUBEMX
```

OR Setup:
```sh
# arm-compiler
# Makefile or Cmake
# script
```

**BUILD**

```sh 
# Find compiler
Get-ChildItem -Path C:\ -Recurse -Filter "arm-none-eabi-gcc.exe" -ErrorAction SilentlyContinue | Select-Object FullName

$env:PATH += ";C:\Program Files\Microsoft Visual Studio\2022\Community\Linux\gcc_arm\bin"
```

```sh 
mkdir build
cd build

Remove-Item -Recurse -Force *
cmake -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE="../cmake/gcc-arm-none-eabi.cmake" ..

cmake --build .
```

```FIX
  /* The "READONLY" keyword is only supported in GCC11 and later, remove it if using GCC10 or earlier. */
```
### Start project in Linux host.

