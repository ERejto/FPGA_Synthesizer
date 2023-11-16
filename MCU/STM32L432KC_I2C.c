#include <stm32l432xx.h>


//Sets up I2C as master reciever with proper setting
  // set i2c clk
  // set params
  // pe enable
  // Configure for spi write
  // write to periph for continuos mode bit 5 of 0x05

  //Set Clk in RCC


void I2C_init(void){
    
    RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN; //turn on clocking to i2c1 periph

    I2C1->CR1 |= I2C_CR1_PE; //Enable peripheral
    I2C1->CR1 |= I2C_CR1_RXIE | I2C_CR1_TXIE; // Enable Transfer and Read interrrupts

    I2C1->CR2 &= ~(I2C_CR2_NBYTES); // clear nbytes
    I2C1->CR2 |= 0x0B << I2C_CR2_NBYTES_Pos; // set nbytes to 11
    
    // configure RCC to be 16M
    // I2C->TIMINGR |= I2C_TIMINGR_PRESC_Pos; // not needed? 
    I2C1->TIMINGR |= 0x1 << I2C_TIMINGR_PRESC_Pos;
    I2C1->TIMINGR |= 0x9 << I2C_TIMINGR_SCLL_Pos;
    I2C1->TIMINGR |= 0x3 << I2C_TIMINGR_SCLH_Pos;
    I2C1->TIMINGR |= 0x2 << I2C_TIMINGR_SDADEL_Pos;
    I2C1->TIMINGR |= 0x3 << I2C_TIMINGR_SCLDEL_Pos;
}




//Writes DAC read value to PA5 
char I2C_read(char Value){
  I2C1->CR2 |= 0x20 << 1; // 0100A2A1A0 0x41
  I2C1->CR2 |= I2C_CR2_RD_WRN;
  I2C1->CR2 |= I2C_CR2_START;
  char index = 0;
  char bytes[11]; // maybe different data type
  while (index < 12){
  char read = I2C1->ISR & I2C_ISR_RXNE;
  if (read)
    bytes[index] = I2C1->RXDR; // might not cast to correct value, if getting zeros change this
    index = index + 1;
  }
  return bytes[9]; // assumes reads starting at adress 0 until end. 
}