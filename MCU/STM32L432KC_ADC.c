/*
Kevin Box and Eli Reijto
December 7 2023
kbox@hmc.edu
erejto@hmc.edu 
*/ 
#include "STM32L432KC_ADC.h"

#define ADC_CHANNELS 4 //4 channels



//Sets up ADC with proper settings
void ADC_init(void){
  // enable pins for GPIO
  pinMode(PA3, GPIO_ANALOG); // channel 5
  pinMode(PA4, GPIO_ANALOG); // channel 6
  pinMode(PA5, GPIO_ANALOG); // channel 7
  pinMode(PA6, GPIO_ANALOG); // channel 8
  
  RCC -> AHB2ENR |= RCC_AHB2ENR_ADCEN; // enable the ADC clock

  ADC1_COMMON->CCR |= ADC_CCR_CKMODE_1;

  ADC1->CFGR |= (0b11 << ADC_CFGR_RES_Pos); // set resolution to 6 bits

  ADC1->CR &= ~ADC_CR_DEEPPWD; //Exit Deep power 
  ADC1->CR |= ADC_CR_ADVREGEN; //Enable ADC internal voltage regulator

  //delay_millis(TIM15, 1); // delay for internal V regulator
  
 // want single conversion mode
  //sequence length set to 4
  //write this to change length
  ADC1 -> SQR1 |= 3 << ADC_SQR1_L_Pos;
 // configure for 0-5 channels PA3-7
  ADC1 -> SQR1 |= 0xb << ADC_SQR1_SQ1_Pos; // convert channel 8,  PA6 first
  ADC1 -> SQR1 |= 0xa << ADC_SQR1_SQ2_Pos; // convert channel 9,  PA5 second
  ADC1 -> SQR1 |= 0x9 << ADC_SQR1_SQ3_Pos; // convert channel 10, PA4 third
  ADC1 -> SQR1 |= 0x8 << ADC_SQR1_SQ4_Pos; // convert channel 11, PA3 fourth

  // begin calibration
  ADC1->CR &= ~(ADC_CR_ADCALDIF);
  ADC1->CR |= ADC_CR_ADCAL;

  // Wait for ADCAL to read 0
  while (ADC1->CR & ADC_CR_ADCAL);
  
  // enable adc
  ADC1 -> CR |= ADC_CR_ADEN;
  //wait until ready
  while(!(ADC1 -> ISR &= 1 << ADC_ISR_ADRDY_Pos));

  // set unsigned data offset is off by default
}



//Reads ADC value
void ADC_read(uint16_t *adcVals){
  
  // uint16_t trash;
  // start conversion sequence
  ADC1->CR |= ADC_CR_ADSTART;
  
  // ADC1->ISR |= ADC_ISR_OVR;
  // wait for sample from channel 8+i
  // while(!(ADC1 -> ISR &= 1 << ADC_ISR_EOC_Pos));
  // read data register and store in array
  // trash = (int16_t) ADC1->DR;

 //  ADC1->ISR |= ADC_ISR_OVR;
  // wait for sample from channel 8+i
  while(!(ADC1 -> ISR &= 1 << ADC_ISR_EOC_Pos));
  // read data register and store in array
  adcVals[0] = (int16_t) ADC1->DR;

  // ADC1->ISR |= ADC_ISR_OVR;
  // wait for sample from channel 8+i
  while(!(ADC1 -> ISR &= 1 << ADC_ISR_EOC_Pos));
  // read data register and store in array
  adcVals[1] = (int16_t) ADC1->DR;


 //  ADC1->ISR |= ADC_ISR_OVR;
  // wait for sample from channel 8+i
  while(!(ADC1 -> ISR &= 1 << ADC_ISR_EOC_Pos));
  // read data register and store in array
  adcVals[2] = (int16_t) ADC1->DR;

  // ADC1->ISR |= ADC_ISR_OVR;
  // wait for sample from channel 8+i
  while(!(ADC1 -> ISR &= 1 << ADC_ISR_EOC_Pos));
  // read data register and store in array
  adcVals[3] = (int16_t) ADC1->DR;


  // wait for end of sequence flag
  while(!(ADC1 -> ISR &= 1 << ADC_ISR_EOS_Pos));
  ADC1 -> ISR |= 1 << ADC_ISR_EOC_Pos;


}