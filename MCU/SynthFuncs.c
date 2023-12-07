#include "STM32L432KC.h"

// recursive 16 -> 4 encoder
uint8_t encoder(uint16_t vals) {
 if (vals == 0) return 0;
 else return 1 + encoder(vals>>1);
}


<<<<<<< HEAD
//
void playNote(char note) {
  char readVal = spiSendReceive(note);
  DAC_write(readVal);
=======
void playNote(uint16_t note) {
  digitalWrite(SPI_CE, 1);
  uint16_t readVal = spiSendReceive(note);
  digitalWrite(SPI_CE, 0);
}

void button_read(uint16_t *notesRead){
  notesRead = 0x00;
>>>>>>> 6af40590bf985ed398c7eb40ff9c2bf6d381391c
}