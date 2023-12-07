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
<<<<<<< HEAD
 int main(void) {
   
=======
int main(void) {
  gpioEnable(GPIO_PORT_A);
>>>>>>> 6af40590bf985ed398c7eb40ff9c2bf6d381391c
  RCC->CR &= 1; // MSI off
  RCC->CR &= ~(0xF<<4);
  RCC->CR |= RCC_CR_MSIRANGE_8; //increase clocking 16M?
  RCC->CR |= RCC_CR_MSIRGSEL;
  RCC->CR |= 1; // MSI ON
  RCC->APB2ENR |= (RCC_APB2ENR_TIM15EN);
  initTIM(TIM15);
  RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);
<<<<<<< HEAD
  //DAC_init();
  //char address = 0x40;
 // uint8_t state = 0; // wait = 0; playing = 1;
  //char note = 0;

/*
SPI TEST CODE
=======
  ADC_init();
  uint16_t adcRead[4];
  uint16_t adcSaved[4];
>>>>>>> 6af40590bf985ed398c7eb40ff9c2bf6d381391c
  // "clock divide" = master clock frequency / desired baud rate
  // the phase for the SPI clock is 1 and the polarity is 0
  initSPI(1, 0, 0);

  uint16_t notesRead[1]; // make it a pointer to pass to function bottom 12 bits one hot encoded for note 
  uint16_t notesSaved; // bottom 12 bits one hot encoded for note 
  uint8_t noteSend; // bottom 4 bits encoded as the note upper 4 bits 0;
  uint16_t sendVal;
  bool updateNote;
  
  // program architucture;
  // Done sequentially 
  // further improvement is operate with Rtos or interupts
  // each while loop, sample the peripherals, if they have changed from previous value save the new value and send to fpga
  // if not check next value
  
  do {
  
  // Read values 
  ADC_read(adcRead);

<<<<<<< HEAD
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
                                                       

=======
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
  if (adcRead[0] != adcSaved[0]) { // if either wavetype changes
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
    sendVal = 0x0000 | (noteSend << 8) | ((adcSaved[0] & (0xF << 2 )) << 2) | ((adcSaved[0] & (0xF << 2 )) >>2);
    //    0x |   0   |  noteSend[3:0]  |         adcSaved[0][5:2]           |        adcSaved[0][5:2]           |

    // SPI send note
    playNote(sendVal);

    //
    updateNote = false;
  }

  } while(1);

/*
  do {
  uint32_t delay = 1;
  uint16_t note = 0x0000;
  delay_millis(TIM15, delay);
  note = 0x0100;
  playNote(note);
  } while(1);
  

  do {
  uint32_t delay = 5000;
  uint16_t note = 0x0000;
  delay_millis(TIM15, delay);
  delay_millis(TIM15, 100);
  note = 0x0100;
  playNote(note);
  
  do {
  ADC_read(result);
  printf("1st, %d     ", result[0]); // PA3
  printf("2nd, %d     ", result[1]); // PA4
  printf("3rd, %d     ", result[2]); // PA5
  printf("4th, %d     \n", result[3]); // PA6
  } while(1);

}while (1); */
>>>>>>> 6af40590bf985ed398c7eb40ff9c2bf6d381391c
}

/*************************** End of file ****************************/