#ifndef STM32L4_I2C_H
#define STM32L4_I2C_H

#include <stdint.h> // Include stdint header
#include <stm32l432xx.h>
#include "STM32L432KC_GPIO.h"

#define RESET_PIN         PB0
#define SCL_PIN           PB6
#define SDO_PIN           PB7
 
#define DEVICE_ADDRESS    0x20 //Address of MCP23008 peripheral 0100A2A1A0

#define IODIR_REGISTER    0x00 //IO Direction Register default 0xFF All inputs
#define IPOL_REGISTER     0x01 //IO Polarity Register writing 1's reverses value
#define GPINTEN_REGISTER  0x02 //GPIO Interrupt enable
#define DEFVAL_REGISTER   0x03 //Sets compare value for interrupts
#define INTCON_REGISTER   0x04 //If set its compared to DEFVAL else compared to previous value
#define IOCON_REGISTER    0x05 //bit5 sequential Operation
                                //bit4 slew rate control bit
                                //bit3 Hardware address enable
                                //bit2 configures interrupt pin as open-drain output
                                //bit1 setss polarity of interrupt pin
#define GPPU_REGISTER     0x06 //Pullup resistor enable register enable all
#define INTF_REGISTER     0x07 //Interrupt flag register
#define INTCAP_REGISTER   0x08 //Saves values of bits at time of interrupt
#define GPIO_REGISTER     0x09 //Read this to know logic value of buttons
#define OLAT_REGISTER     0xA0 //Provides access to output latches

//Initializes DAC with proper settings
void I2C_init(void);

void I2C_write_byte(uint8_t register_address, uint8_t data);

void I2C_setup_peripheral(void);

void I2C_NACKF_check(void);

void I2C_select_register(uint8_t read_register);

//Writes Value to the DAC_Out2 pin PA5 through a buffer
uint8_t I2C_read_byte(void);

#endif