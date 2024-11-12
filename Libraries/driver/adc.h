#include "at32f4xx.h"
#include "at32_board.h"
#include "main.h"

void RCC_Configurations(void);
void adc_init(void);
extern __IO uint16_t ADCConvertedValue[100];


uint8_t short_Cheack(void);
void PE_cheack_init(void);
uint8_t PE_cheack_HL(void);
void PC780_init(void);
uint8_t zhanlian_check(uint8_t LN);
