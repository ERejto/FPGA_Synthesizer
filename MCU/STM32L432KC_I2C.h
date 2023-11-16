#ifndef STM32L4_I2C_H
#define STM32L4_I2C_H

#include <stm32l432xx.h>

//Initializes DAC with proper settings
void I2C_init(void);



//Writes Value to the DAC_Out2 pin PA5 through a buffer
char I2C_read(char Value);

#endif