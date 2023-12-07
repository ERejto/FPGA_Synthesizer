/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include <stdio.h>
#include <stdlib.h>
#include "STM32L432KC.h"
#include "SynthFuncs.h"


/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
 int main(void) {
   
  RCC->CR &= 1; // MSI off
  RCC->CR &= ~(0xF<<4);
  RCC->CR |= RCC_CR_MSIRANGE_8; //increase clocking 16M?
  RCC->CR |= RCC_CR_MSIRGSEL;
  RCC->CR |= 1; // MSI ON
  initTIM(TIM15);
  RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);
  //DAC_init();
  //char address = 0x40;
 // uint8_t state = 0; // wait = 0; playing = 1;
  //char note = 0;

/*
SPI TEST CODE
  // "clock divide" = master clock frequency / desired baud rate
  // the phase for the SPI clock is 1 and the polarity is 0
  initSPI(1, 0, 0);

  do {
  if (state == 0) {
    char buttons = I2C_read(address);
    if (buttons != 0 ) { 
      note = encoder(buttons);
      state = 1;
    }
  }
  else if (state == 1) {
    playNote(note);
    if (I2C_read(address) == 0 ) { 
      playNote(0);
      state = 0;
    }
  } 
}while (1);

*/

/*
ADC TEST CODE
  int data;
  gpioEnable(PA8);
  pinMode(PA8,GPIO_OUTPUT);
  ADC_init();
  while (1){
    data = ADC_read();
    if (data > 100){
      digitalWrite(PA8,1);
    }
  } 
}
*/

  
  gpioEnable(PA8);
  pinMode(PA8,GPIO_OUTPUT);
  gpioEnable(PB1);
  pinMode(PB1,GPIO_OUTPUT);
  uint8_t data;
  I2C_init();

  I2C_setup_peripheral();
  I2C_select_register(GPIO_REGISTER);
  while(1){
    data = I2C_read_byte();
    printf("Data: %d", data);
    if (data == 0x80){
      digitalWrite(PA8,PIO_HIGH);
    }
    else if (data == 0x40){
      digitalWrite(PB1,PIO_HIGH);
    }
   }
                                                       

}

/*************************** End of file ****************************/