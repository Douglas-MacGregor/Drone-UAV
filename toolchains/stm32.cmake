set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -O2")
set(CMAKE_EXE_LINKER_FLAGS "-Tstm32_flash.ld -nostdlib")

set(PLATFORM STM32)