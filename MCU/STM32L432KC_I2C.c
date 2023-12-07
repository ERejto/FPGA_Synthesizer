#include <stm32l432xx.h>
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_I2C.h"



//Sets up I2C as master reciever with proper setting
  // set i2c clk
  // set params
  // pe enable
  // Configure for spi write
  // write to periph for continuos mode bit 5 of 0x05

  //Set Clk in RCC


void I2C_init(void){
    
    RCC->APB1ENR1 |= RCC_APB1ENR1_I2C1EN; //turn on clocking to I2C1 periph

    //SCL Enable
    gpioEnable(SCL_PIN);

    pinMode(SCL_PIN,GPIO_ALT);
    GPIOB -> OTYPER |= 1 << 6;
    GPIOB -> PUPDR |= 1 << GPIO_PUPDR_PUPD6_Pos;
    GPIOB -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6_0 | GPIO_OSPEEDER_OSPEEDR6_1;
    GPIOB -> AFR[0] |= (0b0100 << GPIO_AFRL_AFSEL6_Pos);

    //SDA Enable
    gpioEnable(SDO_PIN);
    GPIOB -> OTYPER |= 1 << 7;
    GPIOB -> PUPDR |= 1 << GPIO_PUPDR_PUPD7_Pos;
    GPIOB -> OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7_0 | GPIO_OSPEEDER_OSPEEDR7_1;

    pinMode(SDO_PIN,GPIO_ALT);
    GPIOB -> AFR[0] |= (0b0100 << GPIO_AFRL_AFSEL7_Pos);
    

    //Reset pin
    gpioEnable(RESET_PIN);
    pinMode(RESET_PIN,GPIO_OUTPUT);
    
    I2C1->CR1 &= ~I2C_CR1_PE; //Disable peripheral


    
    // configure RCC to be 16M
    // I2C->TIMINGR |= I2C_TIMINGR_PRESC_Pos; // not needed? 
    I2C1->TIMINGR |= 0x3 << I2C_TIMINGR_PRESC_Pos;
    I2C1->TIMINGR |= 0x13 << I2C_TIMINGR_SCLL_Pos;
    I2C1->TIMINGR |= 0xF << I2C_TIMINGR_SCLH_Pos;
    I2C1->TIMINGR |= 0x2 << I2C_TIMINGR_SDADEL_Pos;
    I2C1->TIMINGR |= 0x4 << I2C_TIMINGR_SCLDEL_Pos;
    //0x30420F13
    I2C1->CR1 |= I2C_CR1_PE;
    digitalWrite(GPIO_OUTPUT,PIO_LOW);
    I2C1->CR1 |= I2C_CR1_NACKIE| I2C_CR1_RXIE | I2C_CR1_TXIE | I2C_CR1_TCIE; // Enable Transfer and Read interrrupts
    I2C1->CR2 &= ~I2C_CR2_ADD10;
    I2C1->CR2 |= DEVICE_ADDRESS << 1; // 0100A2A1A0 write to device address A2A1A0 user defined
   
}


/*
void I2C_write_byte(uint8_t register_address, uint8_t data, uint8_t max_retries) {
  for (uint8_t retry = 0; retry <= max_retries; ++retry) {
    I2C1->CR2 &= ~I2C_CR2_RD_WRN; // Set to write mode
    I2C1->CR2 &= ~I2C_CR2_NBYTES_Msk; // Reset Num Bytes Bit
    I2C1->CR2 |= (2 << I2C_CR2_NBYTES_Pos); // Num Bytes = 2
    
    I2C1->CR2 |= I2C_CR2_START;

    while (!(I2C1->ISR & I2C_ISR_TXE) | !(I2C1->ISR &I2C_ISR_NACKF));

    if (I2C1->ISR & I2C_ISR_NACKF) {
      // NACK detected, retry or take other actions
      I2C1->ICR |= I2C_ICR_NACKCF;
      if (retry == max_retries) {
        // Max retries reached, handle the error (e.g., log, reset I2C, etc.)
        I2C1->CR1 &= ~I2C_CR1_PE;     // Disable peripheral
        I2C1->CR1 |= I2C_CR1_PE;      // Enable peripheral
        break;
      }
      // Delay or backoff before retrying
      // You may add a delay function here (e.g., delay_ms(10);)
      continue; // Retry the transmission
    }

    I2C1->TXDR = register_address; // Select which register to write to

    while (!(I2C1->ISR & I2C_ISR_TXE) | !(I2C1->ISR &I2C_ISR_NACKF));

    if (I2C1->ISR & I2C_ISR_NACKF) {
      I2C1->ICR |= I2C_ICR_NACKCF;
      // NACK detected, retry or take other actions
      if (retry == max_retries) {
        // Max retries reached, handle the error
        break;
      }
      continue; // Retry the transmission
    }

    I2C1->TXDR = data; // Write data to register_address

    // Wait for TC or NACK
    while (!(I2C1->ISR & I2C_ISR_TC) | !(I2C1->ISR &I2C_ISR_NACKF));

    if (I2C1->ISR & I2C_ISR_NACKF) {
      I2C1->ICR |= I2C_ICR_NACKCF;
      // NACK detected, retry or take other actions
      if (retry == max_retries) {
        // Max retries reached, handle the error
        break;
      }
      continue; // Retry the transmission
    }

    // Successful write, exit the loop
    break;
  }
}

*/

void I2C_write_byte(uint8_t register_address, uint8_t data){
  I2C1->CR2 &= ~I2C_CR2_RD_WRN; //Set to write mode
  I2C1->CR2 &= ~I2C_CR2_NBYTES_Msk; //Reset Num Bytes Bit
  
  I2C1->CR2 |= (2 <<I2C_CR2_NBYTES_Pos); //Num Bytes = 2

  I2C1->CR2 |=  I2C_CR2_START;
  I2C1->ICR |= I2C_ICR_NACKCF;

  while(!(I2C1->ISR & I2C_ISR_TXE));
  I2C1->TXDR = register_address; //Select which register to write to
  
  while(!(I2C1->ISR & I2C_ISR_TXE));
  I2C1->TXDR = data;  //Write data to register_address

  while(!(I2C1->ISR & I2C_ISR_TC));
}

void I2C_setup_peripheral(void){
    I2C_write_byte(IPOL_REGISTER,0xFF); //Reverse all polarity
    I2C_write_byte(GPPU_REGISTER,0xFF);
    I2C_write_byte(IOCON_REGISTER,0x20);
}
 
void I2C_select_register(uint8_t read_register){
  I2C1->CR2 &=  ~I2C_CR2_RD_WRN;
  I2C1->CR2 &= ~I2C_CR2_NBYTES_Msk;
  I2C1->ICR |= I2C_ICR_NACKCF;
  I2C1->CR2 |=  1 << I2C_CR2_NBYTES_Pos;

  I2C1->CR2 |=  I2C_CR2_START;
  
  while(!(I2C1->ISR & I2C_ISR_TXIS));
  
  I2C1->TXDR = read_register;
  while(!(I2C1->ISR & I2C_ISR_TC));
}


uint8_t I2C_read_byte(void){
  I2C1->CR2  |= I2C_CR2_RD_WRN;
  I2C1->CR2 &= ~I2C_CR2_NBYTES_Msk;
  I2C1->CR2 |= 1<<I2C_CR2_NBYTES_Pos;

  I2C1->CR2  |= I2C_CR2_START;

  while(!(I2C1->ISR & I2C_ISR_RXNE));
  uint8_t data = I2C1->RXDR;

  while(!(I2C1->ISR & I2C_ISR_TC));
  return data;
}


/*
uint8_t I2C_read_byte(uint8_t register_address, uint8_t max_retries) {
  uint8_t read_data = 0;

  for (uint8_t retry = 0; retry <= max_retries; ++retry) {
    I2C1->CR2 &= ~I2C_CR2_RD_WRN; // Set to write mode
    I2C1->CR2 &= ~I2C_CR2_NBYTES_Msk; // Reset Num Bytes Bit
    I2C1->CR2 |= (1 << I2C_CR2_NBYTES_Pos); // Num Bytes = 1

    I2C1->CR2 |= I2C_CR2_START;

    while (!(I2C1->ISR & I2C_ISR_TXE) | !(I2C1->ISR &I2C_ISR_NACKF));

    if (I2C1->ISR & I2C_ISR_NACKF) {
      I2C1->ICR |= I2C_ICR_NACKCF;
      // NACK detected, retry or take other actions
      if (retry == max_retries) {
        // Max retries reached, handle the error
        // You can add additional error handling code here
        break;
      }
      continue; // Retry the transmission
    }

    I2C1->TXDR = register_address; // Select which register to read from

    // Wait for TC
    while (!(I2C1->ISR & I2C_ISR_TC) | !(I2C1->ISR &I2C_ISR_NACKF));
    

    // Restart and initiate read
    I2C1->CR2 |= I2C_CR2_RD_WRN;

    while (!(I2C1->ISR & I2C_ISR_RXNE) | !(I2C1->ISR &I2C_ISR_NACKF));
    

    read_data = I2C1->RXDR;

    // Wait for TC or NACK
    while (!(I2C1->ISR & I2C_ISR_TC) | !(I2C1->ISR &I2C_ISR_NACKF));

    if (I2C1->ISR & I2C_ISR_NACKF) {
      I2C1->ICR |= I2C_ICR_NACKCF;
      // NACK detected, retry or take other actions
      if (retry == max_retries) {
        // Max retries reached, handle the error
        break;
      }
      continue; // Retry the transmission
    }

    // Successful read, exit the loop
    break;
  }

  return read_data;
}


*/