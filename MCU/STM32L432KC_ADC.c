#include <stm32l432xx.h>


//Sets up ADC with proper settings
void ADC_init(void){
  RCC -> AHB2ENR |= RCC_AHB2ENR_ADCEN;

  ADC1 -> IER |= ADC_IER_EOCIE 
              |  ADC_IER_EOSIE
              |  ADC_IER_ADRDY;
  
  ADC1 -> CR |= ADC_CR_ADEN;
  //wait until ready
  while(!(ADC1 -> IER &= 1 << ADC_ISR_ADRDY_Pos));

  //sequence length set to 1
  //write this to change length
  //ADC1 -> SQR1 |= length << ADC_SQR1_L_Pos;
  //Will read channel 0 in the sequence
  


}

//Reads ADC value
uint16_t ADC_read(void){
  uint16_t read_value;
  while(ADC1 -> IER &= 1 << ADC_ISR_EOS_Pos){
    while(ADC1 -> IER &= 1<< ADC_ISR_EOC_Pos){
      read_value |= ADC1 ->DR;
    }
  }
    return read_value;
}