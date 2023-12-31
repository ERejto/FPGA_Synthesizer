/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Implements inputs and spi send for the FPGA Sythesizer
Kevin Box and Eli Reijto
December 7 2023
kbox@hmc.edu
erejto@hmc.edu 

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
  

  // configure faster  sys clock
  RCC->CR &= 1; // MSI off
  RCC->CR &= ~(0xF<<4);
  RCC->CR |= RCC_CR_MSIRANGE_8; //increase clocking 16M?
  RCC->CR |= RCC_CR_MSIRGSEL;
  RCC->CR |= 1; // MSI ON

  // enable tim for delays
  RCC->APB2ENR |= (RCC_APB2ENR_TIM15EN);
  initTIM(TIM15);

  // enable GPIO for button read
  RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);
  gpioEnable(GPIO_PORT_A);
  gpioEnable(GPIO_PORT_B);
  gpioEnable(GPIO_PORT_C);
  button_init();
  uint16_t notesRead[1] = {0}; // make it a pointer to pass to function bottom 12 bits one hot encoded for note 
  uint16_t notesSaved = 0; // bottom 12 bits one hot encoded for note 
  uint8_t noteSend = 0; // bottom 4 bits encoded as the note upper 4 bits 0;

  // enable ADC
  ADC_init();
  uint16_t adcRead[4] = {0, 0, 0, 0};
  uint16_t adcSaved[4] = {0, 0, 0, 0};
  // "clock divide" = master clock frequency / desired baud rate
  // the phase for the SPI clock is 1 and the polarity is 0
  initSPI(1, 0, 0);

 
  uint16_t sendVal = 0;
  bool updateNote = false;
  
  // program design;
  // Done sequentially 
  // further improvement is operate with Rtos or interupts
  // each while loop, sample the peripherals, if they have changed from previous value save the new value and send to fpga
  // if not check next value
  
  do {
  
  // Read values 
  ADC_read(adcRead);

  // if either FM setting changes update them on FPGA
  if (adcRead[2] != adcSaved[2] || adcRead[3] != adcSaved[3]) { 

    // save settings
    adcSaved[2]= adcRead[2];
    adcSaved[3]= adcRead[3];

    // construct send (bit swizzle) top 4 bits 0x1, next 6: adcSaved[0][5:0], next 6: adcSaved[0][5:0]
    sendVal = 0x1000 | ((adcSaved[2] & (0x3F)) << 6) | (adcSaved[3] & 0x3F);
    //    0x |   1   |         adcSaved[2][5:0]      |   adcSaved[3][5:0]  |

    // SPI send note
    playNote(sendVal);
  }
  
  // read buttons
  button_read(notesRead);

  // if either wave type changes else if the button read changes update note and settings
  if (adcRead[0] != adcSaved[0]) {
    // save changed value
    adcSaved[0]= adcRead[0];
    
    // set update note
    updateNote = true;
  }  
  if (adcRead[1] != adcSaved[1]) { // if either wavetype changes
    // save changed value
    adcSaved[1]= adcRead[1];
    
    // set update note
    updateNote = true;
  }
  
  //if notes changes save it and encode it
  if (notesRead[0] != notesSaved) {
    notesSaved = notesRead[0]; // need [0] to dereference pointer
    noteSend = encoder(notesSaved);
    updateNote = true;
  }
  
  // if either wave type or notes have changed, 
  if (updateNote) {
    // construct update value (bit swizzle):
    // top 4 bits 0x0, next nibble: bottom 4 bits of noteSend, next nibble: adcSaved[0][5:2], next nibble: adcSaved[1][5:2]. 
    sendVal = 0x0000 | (noteSend << 8) | ((adcSaved[1] & (0xF << 2 )) << 2) | ((adcSaved[0] & (0xF << 2 )) >>2);
    //    0x |   0   |  noteSend[3:0]  |         adcSaved[0][5:2]           |        adcSaved[0][5:2]           |

    // SPI send note
    playNote(sendVal);

    // unset update
    updateNote = false;
  }

  } while(1);
}

/*************************** End of file ****************************/