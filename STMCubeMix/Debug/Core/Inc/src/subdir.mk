################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Inc/src/_amalgamate.c \
../Core/Inc/src/balz.c \
../Core/Inc/src/bcm.c \
../Core/Inc/src/bcm_bwt.c \
../Core/Inc/src/crush.c \
../Core/Inc/src/deflate.c \
../Core/Inc/src/lz4x.c \
../Core/Inc/src/lzma.c \
../Core/Inc/src/lzp1.c \
../Core/Inc/src/lzrw3a.c \
../Core/Inc/src/lzss.c \
../Core/Inc/src/pack.c \
../Core/Inc/src/ppp.c \
../Core/Inc/src/raw.c \
../Core/Inc/src/ulz.c 

OBJS += \
./Core/Inc/src/_amalgamate.o \
./Core/Inc/src/balz.o \
./Core/Inc/src/bcm.o \
./Core/Inc/src/bcm_bwt.o \
./Core/Inc/src/crush.o \
./Core/Inc/src/deflate.o \
./Core/Inc/src/lz4x.o \
./Core/Inc/src/lzma.o \
./Core/Inc/src/lzp1.o \
./Core/Inc/src/lzrw3a.o \
./Core/Inc/src/lzss.o \
./Core/Inc/src/pack.o \
./Core/Inc/src/ppp.o \
./Core/Inc/src/raw.o \
./Core/Inc/src/ulz.o 

C_DEPS += \
./Core/Inc/src/_amalgamate.d \
./Core/Inc/src/balz.d \
./Core/Inc/src/bcm.d \
./Core/Inc/src/bcm_bwt.d \
./Core/Inc/src/crush.d \
./Core/Inc/src/deflate.d \
./Core/Inc/src/lz4x.d \
./Core/Inc/src/lzma.d \
./Core/Inc/src/lzp1.d \
./Core/Inc/src/lzrw3a.d \
./Core/Inc/src/lzss.d \
./Core/Inc/src/pack.d \
./Core/Inc/src/ppp.d \
./Core/Inc/src/raw.d \
./Core/Inc/src/ulz.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Inc/src/%.o Core/Inc/src/%.su: ../Core/Inc/src/%.c Core/Inc/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F051x8 -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Inc-2f-src

clean-Core-2f-Inc-2f-src:
	-$(RM) ./Core/Inc/src/_amalgamate.d ./Core/Inc/src/_amalgamate.o ./Core/Inc/src/_amalgamate.su ./Core/Inc/src/balz.d ./Core/Inc/src/balz.o ./Core/Inc/src/balz.su ./Core/Inc/src/bcm.d ./Core/Inc/src/bcm.o ./Core/Inc/src/bcm.su ./Core/Inc/src/bcm_bwt.d ./Core/Inc/src/bcm_bwt.o ./Core/Inc/src/bcm_bwt.su ./Core/Inc/src/crush.d ./Core/Inc/src/crush.o ./Core/Inc/src/crush.su ./Core/Inc/src/deflate.d ./Core/Inc/src/deflate.o ./Core/Inc/src/deflate.su ./Core/Inc/src/lz4x.d ./Core/Inc/src/lz4x.o ./Core/Inc/src/lz4x.su ./Core/Inc/src/lzma.d ./Core/Inc/src/lzma.o ./Core/Inc/src/lzma.su ./Core/Inc/src/lzp1.d ./Core/Inc/src/lzp1.o ./Core/Inc/src/lzp1.su ./Core/Inc/src/lzrw3a.d ./Core/Inc/src/lzrw3a.o ./Core/Inc/src/lzrw3a.su ./Core/Inc/src/lzss.d ./Core/Inc/src/lzss.o ./Core/Inc/src/lzss.su ./Core/Inc/src/pack.d ./Core/Inc/src/pack.o ./Core/Inc/src/pack.su ./Core/Inc/src/ppp.d ./Core/Inc/src/ppp.o ./Core/Inc/src/ppp.su ./Core/Inc/src/raw.d ./Core/Inc/src/raw.o ./Core/Inc/src/raw.su ./Core/Inc/src/ulz.d ./Core/Inc/src/ulz.o ./Core/Inc/src/ulz.su

.PHONY: clean-Core-2f-Inc-2f-src

