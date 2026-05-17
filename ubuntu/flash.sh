#!/bin/bash

echo "--- FLASH ---"

cd ../stm32f103FreeRTOS_window/build/

# sudo apt install stlink-tools

st-info --probe

arm-none-eabi-objcopy -O ihex stm32f103FreeRTOS.elf stm32f103FreeRTOS.hex
arm-none-eabi-objcopy -O binary stm32f103FreeRTOS.elf stm32f103FreeRTOS.bin

st-flash write stm32f103FreeRTOS.bin 0x08000000
