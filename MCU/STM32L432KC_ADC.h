#ifndef STM32L4_ADC_H
#define STM32L4_ADC_H

#include <stm32l432xx.h>
#include "STM32L432KC.h"

//Initializes DAC with proper settings
void ADC_init(void);



//Writes Value to the DAC_Out2 pin PA5 through a buffer
void ADC_read(uint16_t *adcVals);

#endif