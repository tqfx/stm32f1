#ifndef __MAX7219_H__
#define __MAX7219_H__

#include "main.h"
#include "spi_s.h"

#define MAX7129_PORT GPIOA
#define MAX7129_CS LL_GPIO_PIN_4

void Write_Max7219(uint8_t addr, uint8_t data);
void Max7219_Init(void);

#endif