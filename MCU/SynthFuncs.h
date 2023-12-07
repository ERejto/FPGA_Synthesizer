#ifndef STM32L4_SF_H
#define STM32L4_SF_H

#include <stm32l432xx.h>

// recursive encoder that finds the MSB
uint8_t encoder(uint16_t vals);

//Initializes button GPIO pins
void button_init(void);

// send a 16 bit value over SPI link to the FPGA
void playNote(uint16_t note);

// read 12 buttons in gpio, encode in 1 in notesRead
void button_read(uint16_t *notesRead);

#endif