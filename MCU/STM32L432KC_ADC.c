#include <stm32l432xx.h>


//Sets up DAC with proper settings
void ADC_init(void){
  DAC -> CR |= DAC_CR_EN2;
}

//Writes DAC read value to PA5 
void ADC_write(int8_t Value){
  DAC -> DHR8R2 = Value;
}