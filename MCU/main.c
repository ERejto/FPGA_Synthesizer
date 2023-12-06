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
  gpioEnable(GPIO_PORT_A);
  RCC->CR &= 1; // MSI off
  RCC->CR &= ~(0xF<<4);
  RCC->CR |= RCC_CR_MSIRANGE_8; //increase clocking 16M?
  RCC->CR |= RCC_CR_MSIRGSEL;
  RCC->CR |= 1; // MSI ON
  RCC->APB2ENR |= (RCC_APB2ENR_TIM15EN);
  initTIM(TIM15);
  RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);
  char address = 0x40;
  uint8_t state = 0; // wait = 0; playing = 1;
  

  // "clock divide" = master clock frequency / desired baud rate
  // the phase for the SPI clock is 1 and the polarity is 0
  initSPI(1, 0, 0);
/*
  do {
  uint32_t delay = 1;
  uint16_t note = 0x0000;
  delay_millis(TIM15, delay);
  note = 0x0100;
  playNote(note);
  } while(1);
  */

  do {
  uint32_t delay = 5000;
  uint16_t note = 0x0000;
  delay_millis(TIM15, delay);
  delay_millis(TIM15, 100);
  note = 0x0100;
  playNote(note);

}while (1);
}


/*************************** End of file ****************************/
