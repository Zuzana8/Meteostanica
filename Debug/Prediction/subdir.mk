################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Prediction/zambretti.c 

OBJS += \
./Prediction/zambretti.o 

C_DEPS += \
./Prediction/zambretti.d 


# Each subdirectory must supply rules for building sources it contributes
Prediction/%.o Prediction/%.su Prediction/%.cyclo: ../Prediction/%.c Prediction/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303x8 -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I"C:/Users/szdor/Desktop/I-RK/SEM7/VRS/Semestralny projekt/Marika/Meteostanica/Sensors" -I"C:/Users/szdor/Desktop/I-RK/SEM7/VRS/Semestralny projekt/Marika/Meteostanica/Prediction" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Prediction

clean-Prediction:
	-$(RM) ./Prediction/zambretti.cyclo ./Prediction/zambretti.d ./Prediction/zambretti.o ./Prediction/zambretti.su

.PHONY: clean-Prediction

