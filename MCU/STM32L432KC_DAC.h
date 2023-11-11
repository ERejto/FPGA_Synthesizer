#ifndef STM32L4_DAC_H
#define STM32L4_DAC_H

#include <stm32l432xx.h>

//Initializes DAC with proper settings
void DAC_init(void);



//Writes Value to the DAC_Out2 pin PA5 through a buffer
void DAC_write(int8_t Value);

#endif