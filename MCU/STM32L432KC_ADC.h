#ifndef STM32L4_ADC_H
#define STM32L4_ADC_H

#include <stm32l432xx.h>
#include "STM32L432KC.h"

//Initializes DAC with proper settings
void ADC_init(void);



//Writes Value to the DAC_Out2 pin PA5 through a buffer
<<<<<<< HEAD
uint16_t ADC_read(void);
=======
void ADC_read(uint16_t *adcVals);
>>>>>>> 6af40590bf985ed398c7eb40ff9c2bf6d381391c

#endif