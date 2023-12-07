#include "STM32L432KC.h"

// recursive 16 -> 4 encoder
uint8_t encoder(uint16_t vals) {
 if (vals == 0) return 0;
 else return 1 + encoder(vals>>1);
}
//Cant use :
/*
PA11
PB3
PB5
PB4
PA3
PA4
PA5
PA6
*/


//Using
/*
PA9
PA10
PA12
PB0
PB7
PB6
PB1
PA8
PA7
PA0
PA1
PA2
*/
void button_init(void) {
  gpioEnable(PA0);
  pinMode(PA0,GPIO_INPUT); 
  GPIOA -> PUPD |= 1 << GPIO_PUPDR_PUPD0_Pos;
  gpioEnable(PA1);
  pinMode(PA1,GPIO_INPUT); 
  GPIOA -> PUPD |= 1 << GPIO_PUPDR_PUPD1_Pos;
  gpioEnable(PA2);
  pinMode(PA2,GPIO_INPUT); 
  GPIOA -> PUPD |= 1 << GPIO_PUPDR_PUPD2_Pos;
  gpioEnable(PA7);
  pinMode(PA7,GPIO_INPUT); 
  GPIOA -> PUPD |= 1 << GPIO_PUPDR_PUPD7_Pos;
  gpioEnable(PA8);
  pinMode(PA8,GPIO_INPUT); 
  GPIOA -> PUPD |= 1 << GPIO_PUPDR_PUPD8_Pos;
  gpioEnable(PA9);
  pinMode(PA9,GPIO_INPUT); 
  GPIOA -> PUPD |= 1 << GPIO_PUPDR_PUPD9_Pos;
  gpioEnable(PA10);
  pinMode(PA10,GPIO_INPUT); 
  GPIOA -> PUPD |= 1 << GPIO_PUPDR_PUPD10_Pos;
  gpioEnable(PA12);
  pinMode(PA12,GPIO_INPUT); 
  GPIOA -> PUPD |= 1 << GPIO_PUPDR_PUPD12_Pos;
  gpioEnable(PB0);
  pinMode(PB0,GPIO_INPUT); 
  GPIOB -> PUPD |= 1 << GPIO_PUPDR_PUPD0_Pos;
  gpioEnable(PB1);
  pinMode(PB1,GPIO_INPUT); 
  GPIOB -> PUPD |= 1 << GPIO_PUPDR_PUPD1_Pos;
  gpioEnable(PB6);
  pinMode(PB6,GPIO_INPUT);
  GPIOB -> PUPD |= 1 << GPIO_PUPDR_PUPD6_Pos;
  gpioEnable(PB7);
  pinMode(PB7,GPIO_INPUT); 
  GPIOB -> PUPD |= 1 << GPIO_PUPDR_PUPD7_Pos;
}

void playNote(uint16_t note) {
  digitalWrite(SPI_CE, 1);
  uint16_t readVal = spiSendReceive(note);
  digitalWrite(SPI_CE, 0);
}


/*
PA0 = C4
PA1 = C#4
PA2 = D4
PA7 = D#4
PA8 = E4
PA9 = F4
PA10 = F#4
PA12 = G4
PB0 = G#4
PB1 = A4
PB6 = A#4
PB7 = B4
*/


void button_read(uint16_t *notesRead){
  if (digitalRead(PA0)){
    notesRead[0] |= 1<<0;
  }
  else if(digitalRead(PA1)){
    notesRead[0] |= 1<<1;
  }
  else if(digitalRead(PA2)){
    notesRead[0] |= 1<<2;
  }
  else if(digitalRead(PA7)){
    notesRead[0] |= 1<<3;
  }
  else if(digitalRead(PA8)){
    notesRead[0] |= 1<<4;
  }
  else if(digitalRead(PA9)){
    notesRead[0] |= 1<<5;
  }
  else if(digitalRead(PA10)){
    notesRead[0] |= 1<<6;
  }
  else if(digitalRead(PA12)){
    notesRead[0] |= 1<<7;
  }
  else if(digitalRead(PB0)){
    notesRead[0] |= 1<<8;
  }
  else if(digitalRead(PB1)){
    notesRead[0] |= 1<<9;
  }
  else if(digitalRead(PB6)){
    notesRead[0] |= 1<<10;
  }
  else if(digitalRead(PB7)){
    notesRead[0] |= 1<<11;
  }
}