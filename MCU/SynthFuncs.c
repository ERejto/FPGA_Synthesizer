#include "STM32L432KC.h"


char encoder(char vals) {
 if (vals == 0) return 0;
 else return 1 + encoder(vals>>1);
}

void playNote(char note) {
  char readVal = spiSendReceive(note);
  DAC_write(readVal);
}