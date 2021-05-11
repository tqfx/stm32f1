#ifndef __MAX7219_H__
#define __MAX7219_H__

#include "main.h"

#define MAX7129_PORT GPIOA
#define MAX7129_CS LL_GPIO_PIN_4
#define MAX7129_DMA DMA1
#define MAX7129_DMA_CHANNEL LL_DMA_CHANNEL_3

#define LEN_BUFF 2U
extern uint8_t RxBuffer[LEN_BUFF];
extern uint8_t TxBuffer[LEN_BUFF];

void Write_Max7219(uint8_t addr, uint8_t data);
void Max7119_Display8(uint8_t data[8]);
void Max7219_Init(void);

#endif